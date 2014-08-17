#ifndef _WCE_MPRINTF_H
#define _WCE_MPRINTF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

int xvsnprintf(char *buffer, size_t maxlength, const char *format, va_list ap_save);
int xsnprintf(char *buffer, size_t maxlength, const char *format, ...);
char *xaprintf(const char *format, ...);
char *xvaprintf(const char *format, va_list ap_save);
int xsprintf(char *buffer, const char *format, ...);
int xprintf(const char *format, ...);
int xfprintf(FILE *whereto, const char *format, ...);
int xvsprintf(char *buffer, const char *format, va_list ap_save);
int xvprintf(const char *format, va_list ap_save);
int xvfprintf(FILE *whereto, const char *format, va_list ap_save);
int xvscprintf(const char *format, va_list ap_save);
int xscprintf(const char *format, ...);

#endif /* _WCE_MPRINTF_H */
