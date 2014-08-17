#ifndef _iSeeProxy_H
#define _iSeeProxy_H

#include <wtf/Vector.h>
#include <PlatformString.h>

namespace WebCore {

class iSeeProxy
{
public:
	static bool enabled() { return proxyEnabled; }
	static const CString &address() { return proxyAddress; }
	static unsigned short port() { return proxyPort; }
	static bool exceptableUrl(const String &url);

	static void setEnable(bool b) { proxyEnabled = b; }
	static void setProxy(String addr, unsigned short port, String excps);

private:
	static bool proxyEnabled;
	static CString proxyAddress;
	static unsigned short proxyPort;
	static Vector<String, 1> proxyExceptions;
};

}

#endif
