#include <windows.h>
#include <stdio.h>
#include <stdarg.h>

#define DBG_ON 0

#if DBG_ON
static FILE *gfpLog = NULL;
#endif

void iLogOpen()
{
#if DBG_ON
	gfpLog = fopen("\\iSee.log", "wt");
#endif
}

void iLogClose()
{
#if DBG_ON
	if (gfpLog)
		fclose(gfpLog);
#endif
}

extern "C" {

void iSeeLogA(const char *fmt, ...)
{
#if DBG_ON
	if (gfpLog)
	{
		va_list args;
		fprintf(gfpLog, "[%8u]:", GetTickCount());
		va_start(args, fmt);
		vfprintf(gfpLog, fmt, args);
		va_end(args);
		fflush(gfpLog);
	}
#endif
}

void iSeeLogW(const wchar_t *fmt, ...)
{
#if DBG_ON
	if (gfpLog)
	{
		va_list args;
		fprintf(gfpLog, "[%8u]:", GetTickCount());
		va_start(args, fmt);
		vfwprintf(gfpLog, fmt, args);
		va_end(args);
		fflush(gfpLog);
	}
#endif
}

}
