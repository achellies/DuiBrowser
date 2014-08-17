#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include <curl/curl.h>

static char* months[12] = {
    "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};

static char* wkdays[7] = {
    "Sun","Mon","Tue","Wed","Thu","Fri","Sat"
};

/* ------------------------------------------------------------------------- */


/*	Strings of any length
**	---------------------
*/
int strcasecomp (const char* a, const char* b)
{
    int diff;
    for( ; *a && *b; a++, b++) {
	if ((diff = tolower(*a) - tolower(*b)))
	    return diff;
    }
    if (*a) return 1;			/* a was longer than b */
    if (*b) return -1;			/* a was shorter than b */
    return 0;				/* Exact match */
}


/*	With count limit
**	----------------
*/
int strncasecomp (const char* a, const char* b, int n)
{
	const char *p =a;
	const char *q =b;
	
	for(p=a, q=b;; p++, q++) {
	    int diff;
	    if (p == a+n) return 0;	/*   Match up to n characters */
	    if (!(*p && *q)) return *p - *q;
	    diff = tolower(*p) - tolower(*q);
	    if (diff) return diff;
	}
	/*NOTREACHED*/
}

/*	Find next Field
**	---------------
**	Finds the next RFC822 token in a string
**	On entry,
**	*pstr	points to a string containing a word separated
**		by white white space "," ";" or "=". The word
**		can optionally be quoted using <"> or "<" ">"
**		Comments surrrounded by '(' ')' are filtered out
**
** 	On exit,
**	*pstr	has been moved to the first delimiter past the
**		field
**		THE STRING HAS BEEN MUTILATED by a 0 terminator
**
**	Returns	a pointer to the first word or NULL on error
*/
char* HTNextField (char** pstr)
{
    char* p = *pstr;
    char* start = NULL;
    if (!pstr || !*pstr) return NULL;
    while (1) {
	/* Strip white space and other delimiters */
	while (*p && (isspace((int) *p) || *p==',' || *p==';' || *p=='=')) p++;
	if (!*p) {
	    *pstr = p;
	    return NULL;				   	 /* No field */
	}

	if (*p == '"') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='"'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '<') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='>'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '(') {					  /* Comment */
	    for(;*p && *p!=')'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else {					      /* Spool field */
	    start = p;
	    while(*p && !isspace((int) *p) && *p!=',' && *p!=';' && *p!='=')
		p++;
	    break;						   /* Got it */
	}
    }
    if (*p) *p++ = '\0';
    *pstr = p;
    return start;
}

/*	Find next LWS delimited token
**	-----------------------------
**	On entry,
**	*pstr	points to a string containing a word separated
**		by white white space "," ";" or "=". The word
**		can optionally be quoted using <"> or "<" ">"
**		Comments surrrounded by '(' ')' are filtered out
**
** 	On exit,
**	*pstr	has been moved to the first delimiter past the
**		field
**		THE STRING HAS BEEN MUTILATED by a 0 terminator
**
**	Returns	a pointer to the first word or NULL on error
*/
char* HTNextLWSToken (char** pstr)
{
    char* p = *pstr;
    char* start = NULL;
    if (!pstr || !*pstr) return NULL;

    /* Strip initial white space  */
    while (*p && (isspace((int) *p))) p++;
    if (!*p) {
	*pstr = p;
	return NULL;				   	 /* No field */
    }

    /* Now search for the next white space */
    start = p;
    while(*p && !isspace((int) *p)) p++;

    if (*p) *p++ = '\0';
    *pstr = p;
    return start;
}

/*	Find next Name-value pair
**	-------------------------
**	This is the same as HTNextField but it does not look for '=' as a
**	separator so if there is a name-value pair then both parts are
**	returned.
**	Returns	a pointer to the first word or NULL on error
*/
char* HTNextPair (char** pstr)
{
    char* p = *pstr;
    char* start = NULL;
    if (!pstr || !*pstr) return NULL;
    while (1) {
	/* Strip white space and other delimiters */
	while (*p && (*p==',' || *p==';')) p++;
	if (!*p) {
	    *pstr = p;
	    return NULL;				   	 /* No field */
	}

	if (*p == '"') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='"'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '<') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='>'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '(') {					  /* Comment */
	    for(;*p && *p!=')'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else {					      /* Spool field */
	    start = p;
	    while(*p && *p!=',' && *p!=';')
		p++;
	    break;						   /* Got it */
	}
    }
    if (*p) *p++ = '\0';
    *pstr = p;
    return start;
}

/*	Find next Name-value param
**	--------------------------
**	This is the same as HTNextPair but doesn't look for ','
**	Returns	a pointer to the first word or NULL on error
*/
char* HTNextParam (char** pstr)
{
    char* p = *pstr;
    char* start = NULL;
    if (!pstr || !*pstr) return NULL;
    while (1) {
	/* Strip white space and other delimiters */
	while (*p && *p==';') p++;
	if (!*p) {
	    *pstr = p;
	    return NULL;				   	 /* No field */
	}

	if (*p == '"') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='"'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '<') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='>'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '(') {					  /* Comment */
	    for(;*p && *p!=')'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else {					      /* Spool field */
	    start = p;
	    while (*p && *p!=';')
		p++;
	    break;						   /* Got it */
	}
    }
    if (*p) *p++ = '\0';
    *pstr = p;
    return start;
}

/*	Find next element in a comma separated string
**	---------------------------------------------
**	This is the same as HTNextPair but it does not look for anything
**	else than ',' as separator
**	Returns	a pointer to the first word or NULL on error
*/
char* HTNextElement (char** pstr)
{
    char* p = *pstr;
    char* start = NULL;
    if (!pstr || !*pstr) return NULL;

    /* Strip white space and other delimiters */
    while (*p && ((isspace((int) *p)) || *p==',')) p++;
    if (!*p) {
	*pstr = p;
	return NULL;					   	 /* No field */
    }
    start = p;
    while (1) {
	if (*p == '"') {				     /* quoted field */
	    for(;*p && *p!='"'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else if (*p == '<') {				     /* quoted field */
	    for(;*p && *p!='>'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else if (*p == '(') {					  /* Comment */
	    for(;*p && *p!=')'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else {					      /* Spool field */
	    while (*p && *p!=',') p++;
	    break;						   /* Got it */
	}
    }
    if (*p) *p++ = '\0';
    *pstr = p;
    return start;
}

/*	Find next "/" delimied segment
**	------------------------------
**	This is the same as HTNextField but it includes "/" as a delimiter.
**	Returns	a pointer to the first segment or NULL on error
*/
char* HTNextSegment (char** pstr)
{
    char* p = *pstr;
    char* start = NULL;
    if (!pstr || !*pstr) return NULL;
    while (1) {
	/* Strip white space and other delimiters */
	while (*p && (isspace((int) *p) || *p==',' || *p==';' || *p=='=' || *p=='/')) p++;
	if (!*p) {
	    *pstr = p;
	    return NULL;				   	 /* No field */
	}

	if (*p == '"') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='"'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '<') {				     /* quoted field */
	    start = ++p;
	    for(;*p && *p!='>'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    break;			    /* kr95-10-9: needs to stop here */
	} else if (*p == '(') {					  /* Comment */
	    for(;*p && *p!=')'; p++)
		if (*p == '\\' && *(p+1)) p++;	       /* Skip escaped chars */
	    p++;
	} else {					      /* Spool field */
	    start = p;
	    while(*p && !isspace((int) *p) && *p!=',' && *p!=';' && *p!='=' && *p!='/')
		p++;
	    break;						   /* Got it */
	}
    }
    if (*p) *p++ = '\0';
    *pstr = p;
    return start;
}

/*
**	Find the next s-expression token from a string of characters.
**	We return the name of this expression and the param points to the
**	parameters. 
**
**	NOTE: The string has been mutilated by '/0's
*/
char* HTNextSExp (char** exp, char** param)
{
    char* p = *exp;
    char* name = NULL;
    if (!exp || !*exp) return NULL;
    while (*p && isspace((int) *p)) p++;		/* Strip leading white space */
    if (!*p) {
	*exp = p;
	return NULL;					   	 /* No field */
    }
    if (*p == '{') {					     /* Open bracket */
	int cnt = 1;
	/*
	**  Look for name of this expression. If we find a token then search
	**  for the rest of the expression and remove the end '}'
	*/
	p++;
	if ((name = HTNextField(&p)) == NULL) return NULL;
	while (*p && isspace((int) *p)) p++;
	*param = p;
	while (*p) {
	    if (*p == '{') cnt++;
	    if (*p == '}') cnt--;
	    if (!cnt) {
		*p = '\0';
		break;
	    }
	    p++;
	}
    }
    return name;
}

/*	Date and Time Parser
**	--------------------
**	These functions are taken from the server written by Ari Luotonen
*/
static int make_month (char* s, char** ends)
{
    char* ptr = s;
    while (!isalpha((int) *ptr)) ptr++;
    if (*ptr) {
	int i;
	*ends = ptr+3;		
	for (i=0; i<12; i++)
	    if (!strncasecomp(months[i], ptr, 3)) return i;
    }
    return 0;
}

/*
**	Parse a str in GMT format to a local time time_t representation
**	Four formats are accepted:
**
**		Wkd, 00 Mon 0000 00:00:00 GMT		(rfc1123)
**		Weekday, 00-Mon-00 00:00:00 GMT		(rfc850)
**		Wkd Mon 00 00:00:00 0000 GMT		(ctime)
**		1*DIGIT					(delta-seconds)
*/
time_t HTParseTime (const char* str, bool expand)
{
	return curl_getdate(str, NULL);
}

/*
**	Returns a string pointer to a static area of the current calendar
**	time in RFC 1123 format, for example
**
**		Sun, 06 Nov 1994 08:49:37 GMT
**
**	The result can be given in both local and GMT dependent on the flag
*/
const char*HTDateTimeStr (time_t * calendar, bool local)
{
    static char buf[40];

#ifdef HAVE_STRFTIME
    if (local) {
	/*
	** Solaris 2.3 has a bug so we _must_ use reentrant version
	** Thomas Maslen <tmaslen@verity.com>
	*/
#if defined(HT_REENTRANT) || defined(SOLARIS)
	struct tm loctime;
	localtime_r(calendar, &loctime);
	strftime(buf, 40, "%a, %d %b %Y %H:%M:%S", &loctime);
#else
	struct tm *loctime = localtime(calendar);
	strftime(buf, 40, "%a, %d %b %Y %H:%M:%S", loctime);
#endif /* SOLARIS || HT_REENTRANT */
    } else {
#if defined(HT_REENTRANT) || defined(SOLARIS)
	struct tm gmt;
	gmtime_r(calendar, &gmt);
    	strftime(buf, 40, "%a, %d %b %Y %H:%M:%S GMT", &gmt);
#else
	struct tm *gmt = gmtime(calendar);
    	strftime(buf, 40, "%a, %d %b %Y %H:%M:%S GMT", gmt);
#endif /* SOLARIS || HT_REENTRANT */
    }
#else
    if (local) {
#if defined(HT_REENTRANT)
	struct tm loctime;
	localtime_r(calendar, &loctime);
#else
	struct tm *loctime = localtime(calendar);
#endif /* HT_REENTRANT */
	sprintf(buf,"%s, %02d %s %04d %02d:%02d:%02d",
		wkdays[loctime->tm_wday],
		loctime->tm_mday,
		months[loctime->tm_mon],
		loctime->tm_year + 1900,
		loctime->tm_hour,
		loctime->tm_min,
		loctime->tm_sec);
    } else {
#if defined(HT_REENTRANT) || defined(SOLARIS)
	struct tm gmt;
	gmtime_r(calendar, &gmt);
#else
	struct tm *gmt = gmtime(calendar);
#endif
	sprintf(buf,"%s, %02d %s %04d %02d:%02d:%02d GMT",
		wkdays[gmt->tm_wday],
		gmt->tm_mday,
		months[gmt->tm_mon],
		gmt->tm_year + 1900,
		gmt->tm_hour,
		gmt->tm_min,
		gmt->tm_sec);
    }
#endif
    return buf;
}

/*	HTDateDirStr
**	------------
**	Generates a date string used in directory listings
*/
bool HTDateDirStr (time_t * time, char* str, int len)
{
#ifdef HAVE_STRFTIME
#if defined(HT_REENTRANT) || defined(SOLARIS)
    struct tm loctime;
    localtime_r(time, &loctime);
    strftime(str, len, "%d-%b-%Y %H:%M", &loctime);
    return YES;
#else
    strftime(str, len, "%d-%b-%Y %H:%M", localtime(time));
    return YES;
#endif /* HT_REENTRANT || SOLARIS */
#else
    if (len >= 16) {
	struct tm *loctime = localtime(time);
	sprintf(str,"%02d-%s-%02d %02d:%02d",
		loctime->tm_mday,
		months[loctime->tm_mon],
		loctime->tm_year % 100,
		loctime->tm_hour,
		loctime->tm_min);
	return true;
    }
    return false;
#endif /* HAVE_STRFTIME */		
}

/* 							     	HTNumToStr
**	Converts a long (byte count) to a string
**	----------------------------------------
**	This function was a PAIN!  In computer-world 1K is 1024 bytes
**	and 1M is 1024K -- however, sprintf() still formats in base-10.
**	Therefore I output only until 999, and then start using the
**	next unit.  This doesn't work wrong, it's just a feature.
**	The "str" must be large enough to contain the result.
*/
void HTNumToStr (unsigned long n, char* str, int len)
{
    double size = n/1024.0;
    if (len < 6) {
	*str = '\0';
	return;
    }
    if (n < 1000)
	sprintf(str, "%dK", n>0 ? 1 : 0);
    else if (size + 0.999 < 1000)
	sprintf(str, "%dK", (int)(size + 0.5));
    else if ((size /= 1024) < 9.9)
	sprintf(str, "%.1fM", (size + 0.05));
    else if (size < 1000)
	sprintf(str, "%dM", (int)(size + 0.5));
    else if ((size /= 1024) < 9.9)
	sprintf(str, "%.1fG", (size + 0.05));
    else
	sprintf(str, "%dG", (int)(size + 0.5));
}
