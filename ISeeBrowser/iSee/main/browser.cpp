#include "config.h"

#include <Windows.h>
#include <WinSock2.h>
#include <tchar.h>

#include "SharedBuffer.h"
#include "Page.h"
#include "ResourceHandle.h"
#include "ResourceResponse.h"
#include "Font.h"
#include "Cache.h"
#include "PageCache.h"
#include "InitializeThreading.h"
#include "FileSystem.h"

#include "WebView.h"
#include "iSeeProxy.h"

#ifdef _WIN32_WCE
#include "resourceppc.h"
#else
#include "Resource.h"
#endif
#include "browser.h"
#include "iSeeDefs.h"
#include "CacheFiles.h"

#include <expat.h>
#if USE(ICU_UNICODE)
#include <unicode/udata.h>
#endif

#if defined(HAVE_PTHREAD_H) && defined(PTW32_STATIC_LIB)
#include "pthread.h"
class pthread_win32_process_attach_class
{
public:
	pthread_win32_process_attach_class()
	{
		pthread_win32_process_attach_np();
	}
	~pthread_win32_process_attach_class()
	{
		pthread_win32_thread_detach_np();
		pthread_win32_process_detach_np();
	}
};
static pthread_win32_process_attach_class pthread_win32_process_attach_handle;
#endif

using namespace WebCore;

// 读写配置文件
#define MAX_STACK_DEPTH		4
#define MAX_STR_LEN			260
#define MAX_CONF_LEN		4096
struct XmlConf
{
	struct XmlConf *next;
	struct XmlConf *prev;
	struct XmlConf *parent;
	struct XmlConf *child;
	struct XmlConf *tail;

	WCHAR *name;
	WCHAR **attrs;
};
struct ConfParser
{
	XML_Parser p;
	WCHAR *stackName[MAX_STACK_DEPTH];
	struct XmlConf *stackConf[MAX_STACK_DEPTH];
	struct XmlConf *stackTail[MAX_STACK_DEPTH];
	int topStack;
	struct XmlConf *xmlConf;
	struct XmlConf *xmlTail;	
};
static struct XmlConf *g_xmlConf = NULL;

// 是否出于全屏状态
static bool g_bIsFullScreen = false;

// 建立属性列表
static WCHAR **ConvertAttrstoUnicode(const XML_Char **xmlAttrs)
{
	if (xmlAttrs == NULL)
		return NULL;
	int count;
	for (count = 0; xmlAttrs[count]; count++) ;
	WCHAR **wAttrs = (WCHAR **)malloc(sizeof(WCHAR *) * (count + 1));
	if (NULL == wAttrs)
		return NULL;
	for (int i = 0; i < count; i++)
	{
		wAttrs[i] = wcsdup(xmlAttrs[i]);
	}
	wAttrs[count] = NULL;
	return wAttrs;
}

extern "C"
{

	static void ConfStartDoctypeDecl(void *userData,
		const XML_Char *doctypeName,
		const XML_Char *sysid,
		const XML_Char *pubid,
		int has_internal_subset)
	{
	}

	static void	ConfEndDoctypeDecl(void *userData)
	{
	}

	static void ConfStartElement(void *userData, const XML_Char *name, const XML_Char **attr)
	{
		struct ConfParser *parser = (struct ConfParser *)userData;
		if (parser)
		{
			if (parser->topStack >= MAX_STACK_DEPTH)
				return;
			struct XmlConf *xc = (struct XmlConf *)malloc(sizeof(struct XmlConf));
			if (xc)
			{
				parser->topStack++;
				xc->name = wcsdup(name);
				xc->attrs = ConvertAttrstoUnicode(attr);
				xc->next = xc->prev = xc->child = xc->parent = NULL;
				parser->stackName[parser->topStack] = xc->name;
				parser->stackConf[parser->topStack] = xc;
				parser->stackTail[parser->topStack] = xc;
			}
		}
	}

	static void ConfEndElement(void *userData, const XML_Char *name)
	{
		struct ConfParser *parser = (struct ConfParser *)userData;
		if (parser)
		{
			struct XmlConf *xmlConf = parser->stackConf[parser->topStack];
			parser->topStack--;
			if (parser->topStack >= 0)
			{
				xmlConf->parent = parser->stackTail[parser->topStack];
				if (NULL == parser->stackTail[parser->topStack]->child)
					parser->stackTail[parser->topStack]->child = xmlConf;
				else
				{
					parser->stackTail[parser->topStack]->tail->next = xmlConf;
					xmlConf->prev = parser->stackTail[parser->topStack]->tail;
				}
				parser->stackTail[parser->topStack]->tail = xmlConf;
			}
			else
			{
				if (NULL == parser->xmlConf)
					parser->xmlConf = xmlConf;
				else
				{
					parser->xmlTail->next = xmlConf;
					xmlConf->prev = parser->xmlTail;
				}
				parser->xmlTail = xmlConf;
			}
		}
	}

	static void ConfCharacterData (void *userData, const XML_Char *s, int len)
	{	// 目前不涉及这一部分
	}

};

// 读取配置文件
static bool ReadConfig()
{
	struct ConfParser parser;
	char              *buf = NULL;
	WCHAR             *wbuf = NULL;
	WCHAR             filename[MAX_PATH];
	FILE              *fp;
	int               len;
	bool              r = false;


	wsprintfW(filename, L"%s\\iSee.xml", GetAppPath());
	// 打开文件
	fp = _wfopen(filename, L"rb");
	if (NULL == fp)
		return false;
	
	// 要求将配置文件一次性读入内存，配置文件过大将返回错误
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	if (len > MAX_CONF_LEN)
		goto FAIL;
	fseek(fp, 0, SEEK_SET);
	
	// 读取配置文件
	buf = (char *)malloc(len + 1);
	if (NULL == buf)
		goto FAIL;
	if (len != fread(buf, 1, len, fp))
		goto FAIL;
	buf[len] = 0;

	memset(&parser, 0, sizeof(parser));
	parser.topStack = -1;
	parser.p = XML_ParserCreate(NULL);
	if (!parser.p)
		goto FAIL;
	XML_SetUserData(parser.p, (void *)&parser);

	XML_SetDoctypeDeclHandler(parser.p, ConfStartDoctypeDecl, ConfEndDoctypeDecl);
	XML_SetElementHandler(parser.p, ConfStartElement, ConfEndElement);
	XML_SetCharacterDataHandler(parser.p, ConfCharacterData);
	
	// 转化为UNICODE字符串
	wbuf = (WCHAR *)XML_GetBuffer(parser.p, (len + 1) * sizeof(WCHAR));
	if (NULL == wbuf)
		goto FAIL;
	if ((len = MultiByteToWideChar(
		CP_ACP,
		0,
		buf,
		len,
		wbuf,
		len + 1
		)) == 0)
		goto FAIL;
	free(buf), buf = NULL;

	if (!XML_ParseBuffer(parser.p, len * sizeof(WCHAR), 1))
	{
		iSeeLogA("%s", 
			XML_ErrorString(XML_GetErrorCode(parser.p)));
		goto FAIL;
	}

	r = true;
	g_xmlConf = parser.xmlConf;

FAIL:
	if (parser.p) XML_ParserFree(parser.p);
	if (buf) free(buf);
	fclose(fp);

	return r;
}

// 超找配置节点
static XmlConf *GetConfigNode(XmlConf *child, WCHAR *n1, WCHAR *n2 = NULL, WCHAR *n3 = NULL)
{
	for (XmlConf *xc1 = child; xc1; xc1 = xc1->next)
	{
		if (wcsicmp(n1, xc1->name) == 0)
		{
			if (n2)
			{
				for (XmlConf *xc2 = xc1->child; xc2; xc2 = xc2->next)
				{
					if (wcsicmp(n2, xc2->name) == 0)
					{
						if (n3)
						{
							for (XmlConf *xc3 = xc2->child; xc3; xc3 = xc3->next)
							{
								if (wcsicmp(n3, xc3->name) == 0)
								{
									return xc3;
								}
							}

						}
						else
							return xc2;
					}
				}
			}
			else
				return xc1;
		}
	}
	return NULL;
}

// 获取属性值
static WCHAR *GetConfigAttr(XmlConf *child, WCHAR *attr, WCHAR *n1, WCHAR *n2 = NULL, WCHAR *n3 = NULL)
{
	XmlConf *xc = GetConfigNode(child, n1, n2, n3);
	if (xc && xc->attrs)
	{
		for (int i = 0; xc->attrs[i];)
		{
			if (wcsicmp(xc->attrs[i], attr) == 0)
			{
				return xc->attrs[i + 1];
			}
			if (xc->attrs[i + 1])
				i += 2;
			else // 出错，属性名和值不配对
				break;
		}
	}
	return L"";
}

// 提供外部调用操作配置文件
WCHAR *GetConfigAttr(WCHAR *attr, WCHAR *n1, WCHAR *n2, WCHAR *n3)
{
	return GetConfigAttr(g_xmlConf->child, attr, n1, n2, n3);
}

void WalkConfigNode(WalkConfigCallback WalkCallback, WCHAR *n1, WCHAR *n2)
{
	XmlConf *xc = GetConfigNode(g_xmlConf->child, n1, n2);
	if (xc && xc->child)
	{
		for (XmlConf *p = xc->child; p; p = p->next)
		{
			WalkCallback(p->name, p->attrs);
		}
	}
}

// 获取应用程序启动路径
LPCWSTR GetAppPath()
{
	static WCHAR wszAppPath[MAX_PATH];
	static bool inited = false;
	if (!inited)
	{
		GetModuleFileNameW(NULL, wszAppPath, MAX_PATH);
		// 去掉文件名称
		WCHAR *szFind = wcsrchr(wszAppPath, L'\\');
		if (szFind)
			*szFind = 0;
		iSeeLogW(L"GetAppPath() %s\n", wszAppPath);
		inited = true;
	}
	return wszAppPath;
}

// 全屏状态
void SetFullScreen(bool b)
{
	g_bIsFullScreen = b;
}
bool IsFullScreen()
{
	return g_bIsFullScreen;
}

#if 0
#include "icudt_data.c"
#endif

#if USE(ICU_UNICODE)
static bool icudtDataExists(LPCWSTR szPath)
{
	String fileName(szPath);
	fileName += "\\icudt40l.dat";
	if (!WebCore::fileExists(fileName))
	{
		String msg(L"File '"); msg += fileName; msg += "' missed!";
		MessageBoxW(ghMainWnd, msg.charactersWithNullTermination(), L"iSee Browser", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	return true;
}
#endif

// 将配置文件数据设置到浏览器参数中
static bool SetBrowserConfig()
{
	// 全局应用程序句柄设置
	Page::setInstanceHandle(ghInstance);

	// Enable javascript window.open ??
	WCHAR *szOpenWindow = GetConfigAttr(g_xmlConf->child, L"VALUE", L"BASE", L"OPENWINDOW");
	if (szOpenWindow && wcsicmp(szOpenWindow, L"false") == 0)
		WebView::enableWindowOpen(false);
	
	// 设置ICU数据
#if 0
	UErrorCode err = U_ZERO_ERROR;
	udata_setCommonData(ICUDT_DATA, &err);
#else
#if USE(ICU_UNICODE)
	CHAR szTmp[MAX_PATH];
	if (WideCharToMultiByte(CP_ACP, 0, GetAppPath(), -1, szTmp, MAX_PATH - 1, NULL, NULL) == 0)
		strcpy(szTmp, ".");
	if (!icudtDataExists(GetAppPath()))
		return false;
	u_setDataDirectory(szTmp);
#endif
#endif
	
	// 字体设置
#ifdef _WIN32_WCE
	{
		WCHAR *szAlias = GetConfigAttr(g_xmlConf->child, L"ALIAS", L"FONT", L"FACE");
		WCHAR *szPath = GetConfigAttr(g_xmlConf->child, L"FILE", L"FONT", L"FACE");
		if (szAlias && szPath)
		{
			WCHAR szDefault[MAX_PATH];
			if (*szPath == '\0')
			{
				wsprintf(szDefault, L"%s\\sunglobe.ttf", GetAppPath());
				szPath = szDefault;
			}

			char name[MAX_PATH], value[MAX_PATH];
			if (WideCharToMultiByte(CP_ACP, 0, szAlias, -1, name, MAX_PATH - 1, NULL, NULL) &&
				WideCharToMultiByte(CP_ACP, 0, szPath, -1, value, MAX_PATH - 1, NULL, NULL))
			{
				setenv(name, value);
			}
		}
	}
#else
	{
		char szFontPath[MAX_PATH];
		WCHAR *szAlias = GetConfigAttr(g_xmlConf->child, L"ALIAS", L"FONT", L"FACE");
		WCHAR *szPath = GetConfigAttr(g_xmlConf->child, L"FILE", L"FONT", L"FACE");
		if (szAlias && szPath)
		{
			if (!WebCore::fileExists(String(szPath)))
			{
#if 0
				// disable by achellies
				String msg(L"File '"); msg += szPath; msg += "' missed!";
				MessageBoxW(ghMainWnd, msg.charactersWithNullTermination(), L"iSee Browser", MB_OK | MB_ICONINFORMATION);
				return false;
#else
				WCHAR szDefault[MAX_PATH];
				if (*szPath == '\0')
				{
					wsprintf(szDefault, L"%s\\sunglobe.ttf", GetAppPath());
					szPath = szDefault;
				}
#endif
			}

			char name[MAX_PATH], value[MAX_PATH];
			if (WideCharToMultiByte(CP_ACP, 0, szAlias, -1, name, MAX_PATH - 1, NULL, NULL) &&
				WideCharToMultiByte(CP_ACP, 0, szPath, -1, value, MAX_PATH - 1, NULL, NULL))
			{
				sprintf(szFontPath, "%s=%s", name, value);
				putenv(szFontPath);
			}
		}
	}
#endif
	
	// PROXY设置
	WCHAR *szEnable = GetConfigAttr(g_xmlConf->child, L"VALUE", L"PROXY", L"ENABLE");
	if (szEnable && wcsicmp(szEnable, L"true") == 0)
		iSeeProxy::setEnable(true);
	else
		iSeeProxy::setEnable(false);
	WCHAR *szAddr = GetConfigAttr(g_xmlConf->child, L"VALUE", L"PROXY", L"ADDRESS");
	if (szAddr)
	{
		// 找一下端口号
		int nPort = 80;
		WCHAR *ppos = wcsrchr(szAddr, L':');
		if (ppos)
		{
			swscanf(ppos + 1, L"%d", &nPort);
			if (nPort <= 0 && nPort >= 0xFFFF)
				nPort = 80;
			*ppos = 0;
		}
		// 例外
		WCHAR *szException = GetConfigAttr(g_xmlConf->child, L"VALUE", L"PROXY", L"EXCEPTION");
		iSeeProxy::setProxy(String(szAddr).charactersWithNullTermination(), nPort, String(szException).charactersWithNullTermination());
	}
	else
		iSeeProxy::setEnable(false);

	// 设置载入页面错误的显示字串
	WebView::setErrorStrings(
		GetConfigAttr(g_xmlConf->child, L"VALUE", L"ERROR", L"TITLE"),
		GetConfigAttr(g_xmlConf->child, L"VALUE", L"ERROR", L"DISPLAY"));
	
	// 内存缓冲设置
	pageCache()->setCapacity(1); // 只缓冲最近的一个页面
	WCHAR *szCache = GetConfigAttr(g_xmlConf->child, L"VALUE", L"BASE", L"CACHESIZE");
	int nCache = 8;
	if (szCache)
		swscanf(szCache, L"%d", &nCache);
	if (nCache < 8) nCache = 8;
	nCache *= 1024 * 1024;
	cache()->setCapacities(0, nCache, nCache);

	// 设置文件缓冲目录
	CacheFiles::startCaches(String(GetAppPath()) + "\\caches", 20);

	return true;
}

// 全局函数
PassRefPtr<SharedBuffer> loadResourceIntoBuffer(const char* name)
{
	int idr;
#if 0 // FIXME:加入这几个图像
	// temporary hack to get resource id
	if (!strcmp(name, "textAreaResizeCorner"))
		idr = IDR_RESIZE_CORNER;
	else if (!strcmp(name, "missingImage"))
		idr = IDR_MISSING_IMAGE;
	else if (!strcmp(name, "urlIcon"))
		idr = IDR_URL_ICON;
	else if (!strcmp(name, "nullPlugin"))
		idr = IDR_NULL_PLUGIN;
	else
#endif
		return 0;

	HRSRC resInfo = FindResource(ghInstance, MAKEINTRESOURCE(idr), L"PNG");
	if (!resInfo)
		return 0;
	HANDLE res = LoadResource(ghInstance, resInfo);
	if (!res)
		return 0;
	void* resource = LockResource(res);
	if (!resource)
		return 0;
	int size = SizeofResource(ghInstance, resInfo);

	return SharedBuffer::create(reinterpret_cast<const char*>(resource), size);
}

bool InitBrowser()
{
	// 启动日志
	extern void iLogOpen();
	iLogOpen();

	iSeeLogA("InitBrowser() %u\n", GetCurrentThreadId());

	// 设置为浏览器主线程
	JSC::initializeThreading(); // Moved from WebView.cpp

	// 读入配置文件
	if (!ReadConfig())
	{
		MessageBoxW(ghMainWnd, L"File 'config.xml' missed!", L"iSee Browser", MB_OK | MB_ICONINFORMATION);
		return false;
	}

	// 将配置文件数据设置到浏览器相关的参数中
	if (!SetBrowserConfig())
		return false;

	return true;
}

bool StartBrowser()
{
	LPWSTR url = GetConfigAttr(g_xmlConf->child, L"VALUE", L"BASE", L"HOMEURL");
	if (url == NULL)
		url = L"";
	if (*url) // 设置地址输入框
		SendMessage(ghMainWnd, WM_SET_URL, (WPARAM)::wcslen(url), (LPARAM)url);
	return WebView::newWebView(url, L"", L"Default") ? true : false;
}

void ExitBrowser()
{
	extern void iLogClose();
	DestroyWindow(GetViewWindow());
	CacheFiles::endCaches();
	iLogClose();
}

HWND GetViewWindow()
{
	HWND hwnd = WebView::getViewWindow();
	if (NULL == hwnd)
		hwnd = WebView::initViewWindow();
	return hwnd;
}

int GetViewsCount()
{
	return WebView::countViews();
}

void LoadTabView(LPCWSTR url, int)
{
	WebView *webView = WebView::currentView();
	if (0 == webView)
	{
		WebView::newWebView(url, L"", L"Default");
	}
	else
		webView->load(url);
}

void ReloadTabView()
{
	WebView *webView = WebView::currentView();
	if (webView)
		webView->reload();
}

void StopTabView()
{
	WebView *webView = WebView::currentView();
	if (webView)
		webView->stop();
}

bool NewTabWindow(LPCWSTR url)
{
	return WebView::newWebView(url, L"", L"Default") ? true : false;
}

// 从菜单操作以后进行关闭操作
void CloseTabWindow(HMENU hMenu, UINT idCommnad)
{
	MENUITEMINFO mii;
	memset(&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_DATA;
	int nLoops = GetViewsCount();
	for (int i = 0; i < nLoops; i++)
	{
		if (GetMenuItemInfo(hMenu, i, TRUE, &mii))
		{
			if (mii.wID == idCommnad)
			{
				WebView::deleteWebView((WebView *)mii.dwItemData);
				break;
			}
		}
	}
}

void ActiveTabView(void *vv)
{
	if (vv)
	{
		WebView *view = (WebView *)vv;
		view->activeView();
	}
}

// 菜单和视图的对应操作
static UINT tabCommands[MAX_VIEM_NUM] = 
#ifdef _WIN32_WCE
{0, 0, 0, 0};
#else
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
#define MAX_TITLE_LEN	20

// 获取View快捷编号
LPWSTR GetViewShotcut(UINT nPos, LPCWSTR wszTitle, UINT nMaxLen, LPWSTR wszMenu, UINT *nOutLen)
{
	int nLen = wcslen(wszTitle);
	if (nPos >= 36)
	{
		if (nLen <= nMaxLen + 2)
			*nOutLen = swprintf(wszMenu, L"%s", wszTitle);
		else
		{
			_snwprintf(wszMenu, nMaxLen, L"%s", wszTitle);
			WCHAR *wszNext = wszMenu + nMaxLen;
			*wszNext = 0;
			wcscat(wszNext, L"..");
			*nOutLen = wszNext - wszMenu + 2;
		}
	}
	else
	{
		if (nLen <= nMaxLen)
			*nOutLen = swprintf(wszMenu, L"&%c.%s", (nPos >= 10) ? ('A' + nPos - 10) : (nPos + '0'), wszTitle);
		else
		{
			_snwprintf(wszMenu, nMaxLen + 2, L"&%c.%s", (nPos >= 10) ? ('A' + nPos - 10) : (nPos + '0'), wszTitle);
			WCHAR *wszNext = wszMenu + nMaxLen + 2;
			*wszNext = 0;
			wcscat(wszNext, L"..");
			*nOutLen = wszNext - wszMenu + 2;
		}
	}
	return wszMenu;
}

// 增加一个View
UINT AddTabView(HMENU hMenu, void *vv, UINT nCurrWindow)
{
	WebView *view = (WebView *)vv;
	WCHAR wszMenuStr[MAX_TITLE_LEN + 5]; // &0 + .. + 结束字符
	MENUITEMINFO mii;
	memset(&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_DATA;

	view->didReceiveTitle(String(L"New Page"), false);
	if (GetViewsCount() == 1)
	{
		mii.fMask |= MIIM_TYPE;
		mii.fType = MFT_STRING;
		mii.dwItemData = (DWORD)vv;
		tabCommands[0] = IDM_TAB_WINDOW + 0;
		mii.wID = tabCommands[0];
		mii.dwTypeData = GetViewShotcut(0, view->getTitle(), MAX_TITLE_LEN, wszMenuStr, &mii.cch);
		SetMenuItemInfo(hMenu, 0, TRUE, &mii);
		CheckMenuItem(hMenu, 0, MF_BYPOSITION | MF_CHECKED);
		return tabCommands[0];
	}
	else
	{
		int i;
		for (i = 0; i < MAX_VIEM_NUM; i++)
		{
			if (tabCommands[i] == 0)
			{
				tabCommands[i] = IDM_TAB_WINDOW + i;
				break;
			}
		}
		if (i < MAX_VIEM_NUM)
		{
			UINT nLen = 0;
			int nLoops = GetViewsCount();
			if (AppendMenu(hMenu, MF_STRING, tabCommands[i], GetViewShotcut(nLoops - 1, view->getTitle(), MAX_TITLE_LEN, wszMenuStr, &nLen)))
			{
				int nBreaks = 0;
				for (int k = 0; k < nLoops; k++)
				{
					if (GetMenuItemInfo(hMenu, k, TRUE, &mii))
					{
						if (mii.wID == nCurrWindow)
						{
							CheckMenuItem(hMenu, k, MF_BYPOSITION | MF_UNCHECKED);
							nBreaks++;
						}
						else if (mii.wID == tabCommands[i])
						{
							mii.dwItemData = (DWORD)vv;
							mii.wID = tabCommands[i];
							SetMenuItemInfo(hMenu, k, TRUE, &mii);
							CheckMenuItem(hMenu, k, MF_BYPOSITION | MF_CHECKED);
							nBreaks++;
						}
					}
					if (nBreaks >= 2)
						break;
				}
				return tabCommands[i];
			}
		}
		else
		{
			WebView::deleteWebView((WebView *)vv);
		}
	}
	return 0;
}

UINT DelTabView(HMENU hMenu, void *vv)
{
	WCHAR wszMenuStr[MAX_TITLE_LEN + 5]; // &0 + .. + 结束字符
	MENUITEMINFO mii;
	memset(&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_DATA;

	if (GetViewsCount() == 0)
	{
		if (GetMenuItemInfo(hMenu, 0, TRUE, &mii))
		{
			UINT idCmd = mii.wID;
			mii.fMask = MIIM_ID | MIIM_DATA | MIIM_TYPE;
			mii.fType = MFT_STRING;
			mii.dwItemData = 0;
			mii.wID = 0;
			mii.dwTypeData = GetViewShotcut(0, L"Blank", MAX_TITLE_LEN, wszMenuStr, &mii.cch);
			SetMenuItemInfo(hMenu, 0, TRUE, &mii);
			tabCommands[idCmd - IDM_TAB_WINDOW] = 0;
			CheckMenuItem(hMenu, 0, MF_BYPOSITION | MF_UNCHECKED);
		}
		return 0;
	}
	else
	{
		int nPos = -1;
		int nLoops = GetViewsCount() + 1; // 列表中已经删除一个，需要加1
		UINT wID = 0;
		for (int i = 0; i < nLoops; i++)
		{
			mii.fMask = MIIM_ID | MIIM_DATA;
			if (GetMenuItemInfo(hMenu, i, TRUE, &mii))
			{
				if (nPos >= 0)
				{
					WebView *view = (WebView *)mii.dwItemData;
					if (view)
					{
						mii.fMask = MIIM_TYPE;
						mii.fType = MFT_STRING;
						mii.dwItemData = 0;
						mii.wID = 0;
						mii.dwTypeData = GetViewShotcut(nPos, view->getTitle(), MAX_TITLE_LEN, wszMenuStr, &mii.cch);
						SetMenuItemInfo(hMenu, nPos, TRUE, &mii);
					}
					nPos++;
				}
				else if (mii.dwItemData == (DWORD)vv)
				{
					DeleteMenu(hMenu, i, MF_BYPOSITION);
					tabCommands[mii.wID - IDM_TAB_WINDOW] = 0;
					if (i == 0)
					{
						if (GetMenuItemInfo(hMenu, i, TRUE, &mii))
						{
							CheckMenuItem(hMenu, i, MF_BYPOSITION | MF_CHECKED);
							ActiveTabView((WebView *)mii.dwItemData);
							wID = mii.wID;
						}
					}
					else
					{
						if (GetMenuItemInfo(hMenu, i - 1, TRUE, &mii))
						{
							CheckMenuItem(hMenu, i - 1, MF_BYPOSITION | MF_CHECKED);
							ActiveTabView((WebView *)mii.dwItemData);
							wID = mii.wID;
						}
					}
					nPos = i;
					i--, nLoops--;
				}
			}
		}
		return wID;
	}
}

void setViewFocus(HMENU, UINT)
{
	WebView::setViewFocus();
}

void SetViewTitle(HMENU hMenu, void *vv)
{
	MENUITEMINFO mii;
	memset(&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_DATA;

	int nLoops = GetViewsCount();
	for (int i = 0; i < nLoops; i++)
	{
		if (GetMenuItemInfo(hMenu, i, TRUE, &mii))
		{
			if (mii.dwItemData == (DWORD)vv)
			{
				WCHAR wszMenuStr[MAX_TITLE_LEN + 5]; // &0 + .. + 结束字符
				WebView *view = (WebView *)vv;

				mii.fMask = MIIM_TYPE;
				mii.fType = MFT_STRING;
				mii.dwItemData = 0;
				mii.wID = 0;
				mii.dwTypeData = GetViewShotcut(i, view->getTitle(), MAX_TITLE_LEN, wszMenuStr, &mii.cch);
				SetMenuItemInfo(hMenu, i, TRUE, &mii);

				break;
			}
		}
	}
}

void GoTabForward()
{
	WebView *webView = WebView::currentView();
	if (webView)
		webView->goForward();
}

void GoTabBack()
{
	WebView *webView = WebView::currentView();
	if (webView)
		webView->goBack();
}

bool IsHistoryBegin()
{
	WebView *webView = WebView::currentView();
	if (webView)
		return webView->historyBegin();
	return true;
}

bool IsHistoryEnd()
{
	WebView *webView = WebView::currentView();
	if (webView)
		return webView->historyEnd();
	return true;
}

// Message handler for javascript dialog box.
static
INT_PTR CALLBACK AlertProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_TITLE_TEXT, (LPCWSTR)lParam);
		SetDlgItemText(hDlg, IDOK, GetConfigAttr(L"OK", L"UI", L"DIALOG"));
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, message);
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
void RunJavaScriptAlert(LPCWSTR message)
{
	DialogBoxParam(ghInstance, MAKEINTRESOURCE(IDD_ALERT), ghMainWnd, AlertProc, (LPARAM)message);
}

static
INT_PTR CALLBACK ConfirmProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_TITLE_TEXT, (LPCWSTR)lParam);
		SetDlgItemText(hDlg, IDOK, GetConfigAttr(L"OK", L"UI", L"DIALOG"));
		SetDlgItemText(hDlg, IDCANCEL, GetConfigAttr(L"CANCEL", L"UI", L"DIALOG"));
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, message);
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
bool RunJavaScriptConfirm(LPCWSTR message)
{
	INT_PTR r = DialogBoxParam(ghInstance, MAKEINTRESOURCE(IDD_CONFIRM), ghMainWnd, ConfirmProc, (LPARAM)message);
	return (r == IDOK ? true : false);
}

// 单行输入对话框
struct MyDlgParam
{
	LPCWSTR message;
	LPCWSTR defaultValue;
	LPWSTR  result;
	UINT    length;
};
static
INT_PTR CALLBACK PromptProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			MyDlgParam *dlgParam = (MyDlgParam *)lParam;
			SetDlgItemText(hDlg, IDC_PROMPT_TITLE, dlgParam->message);
			SetDlgItemText(hDlg, IDC_PROMPT_TEXT, dlgParam->defaultValue);
			SetDlgItemText(hDlg, IDOK, GetConfigAttr(L"OK", L"UI", L"DIALOG"));
			SetDlgItemText(hDlg, IDCANCEL, GetConfigAttr(L"CANCEL", L"UI", L"DIALOG"));
#ifdef _WIN32_WCE
			SetWindowLong(hDlg, DWL_USER, (LONG)lParam);
#else
			SetWindowLongPtr(hDlg, DWL_USER, (LONG_PTR)lParam);
#endif
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if (LOWORD(wParam) == IDOK)
			{
				MyDlgParam *dlgParam = (MyDlgParam *)
#ifdef _WIN32_WCE
					GetWindowLong(hDlg, DWL_USER);
#else
					GetWindowLongPtr(hDlg, DWL_USER);
#endif
				HWND hInput = GetDlgItem(hDlg, IDC_PROMPT_TEXT);
				if (dlgParam && hInput)
				{
					dlgParam->length = GetWindowTextLength(hInput);
					dlgParam->result = (LPWSTR)malloc((dlgParam->length + 1) * sizeof(WCHAR));
					if (dlgParam->result)
						GetWindowText(hInput, dlgParam->result, dlgParam->length + 1);
				}
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, message);
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}
LPWSTR RunJavaScriptPrompt(LPCWSTR message, LPCWSTR defaultValue, UINT &nLen)
{
	MyDlgParam dlgParam;
	dlgParam.message = message;
	dlgParam.defaultValue = defaultValue;
	dlgParam.result = NULL;
	DialogBoxParam(ghInstance, MAKEINTRESOURCE(IDD_PROMPT), ghMainWnd, PromptProc, (LPARAM)&dlgParam);
	nLen = dlgParam.length;
	return dlgParam.result;
}

bool ProxyEnabled()
{
	return iSeeProxy::enabled();
}

void SetProxyEnable(bool b)
{
	iSeeProxy::setEnable(b);
}
