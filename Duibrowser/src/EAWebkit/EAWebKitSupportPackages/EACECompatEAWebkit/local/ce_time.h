#ifndef __CE_TIME_H__
#define __CE_TIME_H__

#if defined(_WIN32_WCE) && _WIN32_WCE >= 0x500
/* we need to prototype the time functions for Windows CE >= 5.0 */
#include <crtdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

struct tm;

time_t time(time_t* timer);
time_t mktime(struct tm *t);
size_t strftime(char * const s, const size_t maxsize, const char * const format, const struct tm * const t);
struct tm *localtime(const time_t *timer);

struct tm * gmtime(const time_t *timer);

_CRTIMP clock_t __cdecl clock(void);

#ifdef __cplusplus
}   /* closing brace for extern "C" */
#endif

#endif /* defined(_WIN32_WCE) && _WIN32_WCE >= 0x600 */

#endif /* !defined(__CE_TIME_H__) */
