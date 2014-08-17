#ifndef _ISEELOG_H
#define _ISEELOG_H

#ifdef  __cplusplus
extern "C" {
#endif

void iSeeLogA(const char *fmt, ...);
void iSeeLogW(const wchar_t *fmt, ...);

#ifdef  __cplusplus
}
#endif

#endif // _ISEELOG_H
