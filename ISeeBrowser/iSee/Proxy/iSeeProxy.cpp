#include "config.h"
#include "iSeeProxy.h"
#include "CString.h"

namespace WebCore {

bool iSeeProxy::proxyEnabled = false;
CString iSeeProxy::proxyAddress = "";
unsigned short iSeeProxy::proxyPort = 8080;
Vector<String, 1> iSeeProxy::proxyExceptions;

bool iSeeProxy::exceptableUrl(const String &url)
{
	for (int i = 0; i < proxyExceptions.size(); i++)
	{
		if (url.find(proxyExceptions[i], 0, false) >= 0)
			return true;
	}
	return false;
}

void iSeeProxy::setProxy(String addr, unsigned short port, String excps)
{
	proxyAddress = addr.latin1();
	proxyPort = port;

	// 设置例外地址
	proxyExceptions.clear();
	int prev = 0, pos = 0;
	while (pos >= 0)
	{
		pos = excps.find(';', pos);
		if (pos >= 0)
		{
			String str = excps.substring(prev, pos - prev);
			if (!str.isEmpty())
			{
				if (str[0] == '*')
					str.remove(0, 1);
				else if (str[str.length() - 1] == '*')
					str.remove(str.length() - 1, 1);
				proxyExceptions.append(str);
			}
			prev = pos + 1;
			pos++;
		}
	}
	// 最后一个
	String str = excps.substring(prev);
	if (!str.isEmpty())
	{
		if (str[0] == '*')
			str.remove(0, 1);
		else if (str[str.length() - 1] == '*')
			str.remove(str.length() - 1, 1);
		proxyExceptions.append(str);
	}
}

}
