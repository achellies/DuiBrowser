#ifndef _SYS_TIME_H_
#define _SYS_TIME_H_

#undef INCL_WINSOCK_API_PROTOTYPES
#define INCL_WINSOCK_API_PROTOTYPES 1 //for ntohl(), etc.
#include <winsock2.h> //timeval

#include <time.h>

struct itimerval {
	struct timeval it_interval; 
	struct timeval it_value; 
};

#ifdef __cplusplus
extern "C" {
#endif

int gettimeofday(struct timeval *p, struct timezone *tz);
int settimeofday(const struct timeval *, const struct timezone *); 

#ifdef __cplusplus
}
#endif

#endif //_SYS_TIME_H_
