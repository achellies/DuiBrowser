#ifndef WCE_EXTS_H
#define WCE_EXTS_H

#ifndef WIN32_LEAN_AND_MEAN
// WIN32_LEAN_AND_MEAN implies NOCRYPT and NOGDI.
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shlwapi.h>

// 常量定义
/*
 * The "real" ancestor window
 */
#define GA_PARENT           1
#define GA_ROOT             2
#define GA_ROOTOWNER        3

#define USER_TIMER_MAXIMUM  0x7FFFFFFF
#define USER_TIMER_MINIMUM  0x0000000A

#define TLS_OUT_OF_INDEXES  ((DWORD)0xFFFFFFFF)
#define INVALID_FILE_ATTRIBUTES ((DWORD)0xFFFFFFFF)

#define HWND_MESSAGE        NULL

#define MAKEPOINTS(l)       (*((POINTS FAR *)&(l)))

/* StretchBlt() Modes */
#define BLACKONWHITE                 1
#define WHITEONBLACK                 2
#define COLORONCOLOR                 3
#define HALFTONE                     4
#define MAXSTRETCHBLTMODE            4

// 结构和函数定义

typedef struct wce_GLYPHMETRICS
{ 
  UINT  gmBlackBoxX; 
  UINT  gmBlackBoxY; 
  POINT gmptGlyphOrigin; 
  short gmCellIncX; 
  short gmCellIncY; 
} wce_GLYPHMETRICS; 

typedef struct wce_FIXED { 
  WORD  fract; 
  short value; 
} wce_FIXED; 

typedef struct wce_MAT2 { 
  wce_FIXED eM11; 
  wce_FIXED eM12; 
  wce_FIXED eM21; 
  wce_FIXED eM22; 
} wce_MAT2;

typedef struct wce_PANOSE { 
  BYTE bFamilyType; 
  BYTE bSerifStyle; 
  BYTE bWeight; 
  BYTE bProportion; 
  BYTE bContrast; 
  BYTE bStrokeVariation; 
  BYTE bArmStyle; 
  BYTE bLetterform; 
  BYTE bMidline; 
  BYTE bXHeight; 
} wce_PANOSE;

typedef struct wce_OUTLINETEXTMETRIC 
{ 
  UINT   otmSize; 
  TEXTMETRIC otmTextMetrics; 
  BYTE   otmFiller; 
  wce_PANOSE otmPanoseNumber; 
  UINT   otmfsSelection; 
  UINT   otmfsType; 
  int    otmsCharSlopeRise; 
  int    otmsCharSlopeRun; 
  int    otmItalicAngle; 
  UINT   otmEMSquare; 
  int    otmAscent; 
  int    otmDescent; 
  UINT   otmLineGap; 
  UINT   otmsCapEmHeight; 
  UINT   otmsXHeight; 
  RECT   otmrcFontBox; 
  int    otmMacAscent; 
  int    otmMacDescent; 
  UINT   otmMacLineGap; 
  UINT   otmusMinimumPPEM; 
  POINT  otmptSubscriptSize; 
  POINT  otmptSubscriptOffset; 
  POINT  otmptSuperscriptSize; 
  POINT  otmptSuperscriptOffset; 
  UINT   otmsStrikeoutSize; 
  int    otmsStrikeoutPosition; 
  int    otmsUnderscoreSize; 
  int    otmsUnderscorePosition; 
  PSTR   otmpFamilyName; 
  PSTR   otmpFaceName; 
  PSTR   otmpStyleName; 
  PSTR   otmpFullName; 
} wce_OUTLINETEXTMETRIC;
 
#define GGO_METRICS     0
#define GGO_GLYPH_INDEX 1

// 重新定义
#ifdef assert
#undef assert
#endif
#define assert(exp)	((void)0)

#ifdef GLYPHMETRICS
#undef GLYPHMETRICS
#endif
#define GLYPHMETRICS              wce_GLYPHMETRICS

#ifdef FIXED
#undef FIXED
#endif
#define FIXED                     wce_FIXED

#ifdef MAT2
#undef MAT2
#endif
#define MAT2                      wce_MAT2

#ifdef OUTLINETEXTMETRIC
#undef OUTLINETEXTMETRIC
#endif
#define OUTLINETEXTMETRIC         wce_OUTLINETEXTMETRIC

#ifdef GdiFlush
#undef GdiFlush
#endif
#define GdiFlush                  wce_GdiFlush

#ifdef SetStretchBltMode
#undef SetStretchBltMode
#endif
#define SetStretchBltMode         wce_SetStretchBltMode

#ifdef GetOutlineTextMetrics
#undef GetOutlineTextMetrics
#endif
#define GetOutlineTextMetrics     wce_GetOutlineTextMetrics

#ifdef GetFontData
#undef GetFontData
#endif
#define GetFontData               wce_GetFontData

#ifdef GetGlyphOutlineW
#undef GetGlyphOutlineW
#endif
#define GetGlyphOutlineW          wce_GetGlyphOutlineW

#ifdef InitializeSecurityDescriptor
#undef InitializeSecurityDescriptor
#endif
#define InitializeSecurityDescriptor wce_InitializeSecurityDescriptor

#ifdef SetSecurityDescriptorDacl
#undef SetSecurityDescriptorDacl
#endif
#define SetSecurityDescriptorDacl wce_SetSecurityDescriptorDacl

#ifdef GetThreadLocale
#define GetThreadLocale
#endif
#define GetThreadLocale wce_GetThreadLocale

#ifdef GetSystemTimeAsFileTime
#undef GetSystemTimeAsFileTime
#endif
#define GetSystemTimeAsFileTime   wce_GetSystemTimeAsFileTime

#ifdef GetFullPathName
#undef GetFullPathName
#endif
#define GetFullPathName           wce_GetFullPathName

#ifdef GetMessageTime
#undef GetMessageTime
#endif
#define GetMessageTime            wce_GetMessageTime

#ifdef GetAncestor
#undef GetAncestor
#endif
#define GetAncestor               wce_GetAncestor

#ifdef FrameRect
#undef FrameRect
#endif
#define FrameRect                 wce_FrameRect

#ifdef SHCreateDirectoryEx
#undef SHCreateDirectoryEx
#endif
#define SHCreateDirectoryEx       wce_SHCreateDirectoryEx

#ifdef GetProcAddress
#undef GetProcAddress
#endif
#ifdef UNICODE
#define GetProcAddress            GetProcAddressW
#else
#define GetProcAddress            GetProcAddressA
#endif

#ifdef OutputDebugStringA
#undef OutputDebugStringA
#endif
#define OutputDebugStringA        wce_OutputDebugStringA

#ifdef GetModuleFileNameA
#undef GetModuleFileNameA
#endif
#define GetModuleFileNameA        wce_GetModuleFileNameA

#ifdef GetModuleHandleA
#undef GetModuleHandleA
#endif
#define GetModuleHandleA          wce_GetModuleHandleA

#ifdef CreateFileA
#undef CreateFileA
#endif
#define CreateFileA               wce_CreateFileA

#ifdef CreateFileMappingA
#undef CreateFileMappingA
#endif
#define CreateFileMappingA       wce_CreateFileMappingA

#ifdef RegEnumKeyExA
#undef RegEnumKeyExA
#endif
#define RegEnumKeyExA             wce_RegEnumKeyExA

#ifdef RegOpenKeyExA
#undef RegOpenKeyExA
#endif
#define RegOpenKeyExA             wce_RegOpenKeyExA

#ifdef RegQueryValueExA
#undef RegQueryValueExA
#endif
#define RegQueryValueExA          wce_RegQueryValueExA

#ifdef RegSetValueExA
#undef RegSetValueExA
#endif
#define RegSetValueExA            wce_RegSetValueExA

#ifdef RegCreateKeyExA
#undef RegCreateKeyExA
#endif
#define RegCreateKeyExA           wce_RegCreateKeyExA

#ifdef RegDeleteValueA
#undef RegDeleteValueA
#endif
#define RegDeleteValueA           wce_RegDeleteValueA

#ifdef RegEnumValueA
#undef RegEnumValueA
#endif
#define RegEnumValueA             wce_RegEnumValueA

#ifdef GetVersionExA
#undef GetVersionExA
#endif
#define GetVersionExA             wce_GetVersionExA

#ifdef GetTextMetricsA
#undef GetTextMetricsA
#endif
#define GetTextMetricsA           wce_GetTextMetricsA

#ifdef LoadLibraryA
#undef LoadLibraryA
#endif
#define LoadLibraryA              wce_LoadLibraryA

#ifdef CreateFileForMappingA
#undef CreateFileForMappingA
#endif
#define CreateFileForMappingA     wce_CreateFileForMappingA

#ifdef GetWindowsDirectory
#undef GetWindowsDirectory
#endif
#ifdef UNICODE
#define GetWindowsDirectory       wce_GetWindowsDirectoryW
#else
#define GetWindowsDirectory       wce_GetWindowsDirectoryA
#endif

#ifdef PathFindFileName
#undef PathFindFileName
#endif

#ifdef __cplusplus
extern "C" {
#endif

// win32
WCE_SHUNT_API BOOL wce_GdiFlush(void);
WCE_SHUNT_API int wce_SetStretchBltMode(HDC inDC, int inStretchMode);

WCE_SHUNT_API UINT wce_GetOutlineTextMetrics(HDC inDC, UINT inData, void* outOTM);
WCE_SHUNT_API DWORD wce_GetFontData(HDC inDC, DWORD inTable, DWORD inOffset, LPVOID outBuffer, DWORD inData);
WCE_SHUNT_API DWORD wce_GetGlyphOutlineW(HDC inDC, WCHAR inChar, UINT inFormat, void* inGM, DWORD inBufferSize, LPVOID outBuffer, CONST void* inMAT2);

WCE_SHUNT_API BOOL wce_InitializeSecurityDescriptor(PSECURITY_DESCRIPTOR, DWORD);
WCE_SHUNT_API BOOL wce_SetSecurityDescriptorDacl(PSECURITY_DESCRIPTOR, BOOL, PACL, BOOL);
WCE_SHUNT_API LCID wce_GetThreadLocale(void);

WCE_SHUNT_API void wce_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime);
WCE_SHUNT_API DWORD wce_GetFullPathName(LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR *lpFilePart);

WCE_SHUNT_API LONG wce_GetMessageTime(void);

WCE_SHUNT_API HWND wce_GetAncestor(HWND hwnd, UINT gaFlags);

WCE_SHUNT_API int wce_FrameRect(HDC inDC, CONST RECT *inRect, HBRUSH inBrush);

WCE_SHUNT_API int wce_SHCreateDirectoryEx(HWND hwnd, LPCWSTR pszPath, SECURITY_ATTRIBUTES *psa);

#if _WIN32_WCE < 0x500
WCE_SHUNT_API BOOL IsDebuggerPresent(VOID);
#endif
WCE_SHUNT_API int _CrtDbgReport(int, const char *, int, const char *, const char *, ...);

WCE_SHUNT_API LPVOID GetThreadStackBase(DWORD dwThreadId);

// win32A & win32W
WCE_SHUNT_API VOID wce_OutputDebugStringA(LPCSTR inOutputString);

WCE_SHUNT_API DWORD wce_GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize);
WCE_SHUNT_API HMODULE wce_GetModuleHandleA(const char *lpName);

WCE_SHUNT_API HANDLE wce_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WCE_SHUNT_API HANDLE wce_CreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName);

WCE_SHUNT_API LONG wce_RegEnumKeyExA(HKEY inKey, DWORD inIndex, LPSTR outName, LPDWORD inoutName, LPDWORD inReserved, LPSTR outClass, LPDWORD inoutClass, PFILETIME inLastWriteTime);
WCE_SHUNT_API LONG wce_RegOpenKeyExA(HKEY inKey, LPCSTR inSubKey, DWORD inOptions, REGSAM inSAM, PHKEY outResult);
WCE_SHUNT_API LONG wce_RegQueryValueExA(HKEY inKey, LPCSTR inValueName, LPDWORD inReserved, LPDWORD outType, LPBYTE inoutBData, LPDWORD inoutDData);

WCE_SHUNT_API LONG wce_RegSetValueExA(HKEY hKey, const char *valname, DWORD dwReserved, DWORD dwType, LPBYTE lpData, DWORD dwSize);
WCE_SHUNT_API LONG wce_RegCreateKeyExA(HKEY hKey, const char *subkey, DWORD dwRes, LPSTR lpszClass, DWORD ulOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES sec_att, PHKEY phkResult, DWORD *lpdwDisp);

WCE_SHUNT_API LONG wce_RegDeleteValueA(HKEY hKey, LPCSTR lpValueName);
WCE_SHUNT_API LONG wce_RegEnumValueA(HKEY hKey, DWORD dwIndex, LPSTR lpValueName, LPDWORD lpcbValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);

WCE_SHUNT_API BOOL wce_GetVersionExA(LPOSVERSIONINFOA lpv);

WCE_SHUNT_API BOOL wce_GetTextMetricsA(HDC hdc, LPTEXTMETRICA lptm);

WCE_SHUNT_API HINSTANCE wce_LoadLibraryA(LPCSTR lpLibFileName);

WCE_SHUNT_API HANDLE wce_CreateFileForMappingA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile);
	
WCE_SHUNT_API UINT wce_GetWindowsDirectoryA(LPSTR inBuffer, UINT inSize);
WCE_SHUNT_API UINT wce_GetWindowsDirectoryW(LPWSTR inBuffer, UINT inSize);

WCE_SHUNT_API LPWSTR PathFindFileName(LPCWSTR pPath);

#ifdef __cplusplus
};
#endif

#endif // WCE_EXTS_H
