//
//
// DirectUI - UI Library
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2006-2007 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. These
// source files may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
////
// Acknowledgements :
// Bjarke Viksoe (http://www.viksoe.dk/code/windowless1.htm)
//
//
//
// Beware of bugs.
//
//
//
////////////////////////////////////////////////////////

#ifdef _USRDLL
#ifdef UILIB_EXPORTS
#define UILIB_API __declspec(dllexport)
#else
#define UILIB_API __declspec(dllimport)
#endif
#else
#define UILIB_API
#endif

// Remove pointless warning messages
#ifdef _MSC_VER
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
  #pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
  #pragma warning (disable : 4786) // identifier was truncated
  #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005
  #endif
#endif // _MSC_VER
#ifdef __BORLANDC__
  #pragma option -w-8027		   // function not expanded inline
#endif

#ifdef UNDER_CE
#pragma comment(linker, "/nodefaultlib:libc.lib")
#pragma comment(linker, "/nodefaultlib:libcd.lib")

// NOTE - this value is not strongly correlated to the Windows CE OS version being targeted
#define WINVER _WIN32_WCE

#include <ceconfig.h>
#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#define SHELL_AYGSHELL
#endif

#ifdef _CE_DCOM
#define _ATL_APARTMENT_THREADED
#endif



// Windows Header Files:
#include <windows.h>


#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
#ifndef _DEVICE_RESOLUTION_AWARE
#define _DEVICE_RESOLUTION_AWARE
#endif
#endif

#ifdef _DEVICE_RESOLUTION_AWARE
#include "DeviceResolutionAware.h"
#endif

#if _WIN32_WCE < 0x500 && ( defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP) )
	#pragma comment(lib, "ccrtrtti.lib")
	#ifdef _X86_	
		#if defined(_DEBUG)
			#pragma comment(lib, "libcmtx86d.lib")
		#else
			#pragma comment(lib, "libcmtx86.lib")
		#endif
	#endif
#endif

#include <altcecrt.h>
#else
// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif
#endif

#ifndef STRICT
  #define STRICT 1
#endif

#define _WINSOCKAPI_            // Prevent winsock.h #include's.

#include "PreConfig.h"

#include <vector>
#include <string>
#include <map>
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>
#include <shlwapi.h>
#include <list>

// For compilers lacking Win64 support
#ifndef  GetWindowLongPtr
  #define GetWindowLongPtr   GetWindowLong
  #define SetWindowLongPtr   SetWindowLong
  #define GWLP_WNDPROC       GWL_WNDPROC
  #define GWLP_HINSTANCE     GWL_HINSTANCE
  #define GWLP_ID            GWL_ID
  #define GWLP_USERDATA      GWL_USERDATA
  #define DWLP_DLGPROC       DWL_DLGPROC
  #define DWLP_MSGRESULT     DWL_MSGRESULT
  #define DWLP_USER          DWL_USER
  #define DWORD_PTR          DWORD
  #define LONG_PTR           LONG
  #define ULONG_PTR          LONG
#endif
#ifndef GetClassLongPtr
  #define GetClassLongPtr    GetClassLong
  #define SetClassLongPtr    SetClassLong
  #define GCLP_HBRBACKGROUND GCL_HBRBACKGROUND
  #define GCLP_HCURSOR       GCL_HCURSOR
  #define GCLP_HICON         GCL_HICON
  #define GCLP_HICONSM       GCL_HICONSM
  #define GCLP_HMODULE       GCL_HMODULE
  #define GCLP_MENUNAME      GCL_MENUNAME
  #define GCLP_WNDPROC       GCL_WNDPROC
#endif

// For Visual Studio 6 (without an updated platform SDK) and Dev-C++
#ifndef OPENFILENAME_SIZE_VERSION_400
  #define OPENFILENAME_SIZE_VERSION_400 sizeof(OPENFILENAME)
#endif

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>
#include <assert.h>
#if defined(UI_BUILD_FOR_WIN32) && !defined(UI_BUILD_FOR_WINCE)
#include <crtdbg.h>
#endif
#include <malloc.h>

#ifndef WC_EDIT
#define WC_EDIT _T("Edit")
#endif

// Automatically include the DirectUI namespace
// define NO_USING_DUILIB_NAMESPACE to skip this step
namespace DuiLib {
// tString is a TCHAR std::string
typedef std::basic_string<TCHAR> tString;
}
#ifndef NO_USING_DUILIB_NAMESPACE
	using namespace DuiLib;
	using namespace std;
#endif

#if defined(UI_BUILD_FOR_WINCE)
//#undef SUPPORT_RICHEDIT
#else
#define SUPPORT_RICHEDIT
#endif

#ifdef SUPPORT_RICHEDIT
#include <imm.h>

#if defined(USING_SYSTEM_SUPPLIED_RICHEDIT)
#include <richedit.h>
#else
#include "..\third_party\RichEd20\inc\richedit.h"
#endif
#endif

#include "UIBase.h"
#include "UIManager.h"
#include "UIDelegate.h"
#include "UIControl.h"
#include "UIMarkup.h"
#include "UIDlgBuilder.h"
#include "UIUtil.h"

#include "UIGDIWindow_Win.h"
#include "UISkWindow_Win.h"

#include "UIRender.h"
#include "UIContainer.h"
#include "UIList.h"
#include "UICombo.h"
#include "UICommonControls.h"
#include "UIActiveX.h"
#include "UIRichEdit.h"
#include "UICrack.h"

#include "PostConfig.h"