#ifndef BROWSER_H_
#define BROWSER_H_

extern HINSTANCE ghInstance;
extern HWND ghMainWnd;

LPCWSTR GetAppPath();
void SetFullScreen(bool b);
bool IsFullScreen();

LPWSTR GetViewShotcut(UINT nPos, LPCWSTR wszTitle, UINT nMaxLen, LPWSTR wszMenu, UINT *nOutLen);
LPWSTR ConvertUrl2Str(LPCWSTR wszUrl, int &nLen, LPWSTR wszRes);// From UrlInputBox.cpp

bool InitBrowser();
bool StartBrowser();
void ExitBrowser();
HWND GetViewWindow();
int  GetViewsCount();

bool NewTabWindow(LPCWSTR url);
void CloseTabWindow(HMENU hMenu, UINT idCommnad);

void LoadTabView(LPCWSTR url, int nLen);
void ReloadTabView();
void StopTabView();

void ActiveTabView(void *vv);
UINT AddTabView(HMENU hMenu, void *vv, UINT nCurrWindow);
UINT DelTabView(HMENU hMenu, void *vv);

void setViewFocus(HMENU hMenu, UINT nCurrWindow);
void SetViewTitle(HMENU hMenu, void *vv);

void GoTabForward();
void GoTabBack();
bool IsHistoryBegin();
bool IsHistoryEnd();

extern "C"
{
	typedef void (*WalkConfigCallback)(WCHAR *name, WCHAR **attrs);
};

WCHAR *GetConfigAttr(WCHAR *attr, WCHAR *n1, WCHAR *n2 = NULL, WCHAR *n3 = NULL);
void WalkConfigNode(WalkConfigCallback WalkCallback, WCHAR *n1, WCHAR *n2 = NULL);

void RunJavaScriptAlert(LPCWSTR message);
bool RunJavaScriptConfirm(LPCWSTR message);
LPWSTR RunJavaScriptPrompt(LPCWSTR message, LPCWSTR defaultValue, UINT &nLen);

bool ProxyEnabled();
void SetProxyEnable(bool b);

#endif
