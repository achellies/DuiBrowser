#define USE_WCEEXTS_H
#include "winceinc.h"

#include <locale.h>
#include <errno.h>
#include <winsock2.h>

// 时间相关宏定义

#define TIME_BEGIN_MACRO do {
#define TIME_END_MACRO } while(0)

/*
* FILETIME has an epoch of 1601.
* Precomputed the 1970 epoch so we do not have to below.
*/
#define FILETIME_1970 116444736000000000i64

/*
* Marco to support add/sub/mul/div on a FILETIME level.
*/
#define FILETIME_ARITH(outFileTime, inFileTime, inOperation, inValue) \
	TIME_BEGIN_MACRO \
	ULARGE_INTEGER buffer1; \
	\
	buffer1.LowPart = inFileTime.dwLowDateTime; \
	buffer1.HighPart = inFileTime.dwHighDateTime; \
	buffer1.QuadPart = buffer1.QuadPart inOperation inValue; \
	outFileTime.dwLowDateTime = buffer1.LowPart; \
	outFileTime.dwHighDateTime = buffer1.HighPart; \
	TIME_END_MACRO

/*
* FILETIME is in 100 nanosecond units.
* Provide macros for conversion to other second units.
*/
#define FILETIME_2_MICROSECONDS(outTime, inFileTime) \
	TIME_BEGIN_MACRO \
	ULARGE_INTEGER buffer2; \
	\
	buffer2.LowPart = inFileTime.dwLowDateTime; \
	buffer2.HighPart = inFileTime.dwHighDateTime; \
	outTime = buffer2.QuadPart / 10i64; \
	TIME_END_MACRO
#define FILETIME_2_MILLISECONDS(outTime, inFileTime) \
	TIME_BEGIN_MACRO \
	ULARGE_INTEGER buffer3; \
	\
	buffer3.LowPart = inFileTime.dwLowDateTime; \
	buffer3.HighPart = inFileTime.dwHighDateTime; \
	outTime = buffer3.QuadPart / 10000i64; \
	TIME_END_MACRO
#define FILETIME_2_SECONDS(outTime, inFileTime) \
	TIME_BEGIN_MACRO \
	ULARGE_INTEGER buffer4; \
	\
	buffer4.LowPart = inFileTime.dwLowDateTime; \
	buffer4.HighPart = inFileTime.dwHighDateTime; \
	outTime = buffer4.QuadPart / 10000000i64; \
	TIME_END_MACRO
#define SECONDS_2_FILETIME(outFileTime, inTime) \
	TIME_BEGIN_MACRO \
	ULARGE_INTEGER buffer5; \
	\
	buffer5.QuadPart = (ULONGLONG)inTime * 10000000i64; \
	outFileTime.dwLowDateTime = buffer5.LowPart; \
	outFileTime.dwHighDateTime = buffer5.HighPart; \
	TIME_END_MACRO

/*
* Conversions from FILETIME 1601 epoch time to LIBC 1970 time.epoch.
*/
#define FILETIME_2_time_t(outTimeT, inFileTime) \
	TIME_BEGIN_MACRO \
	FILETIME result6; \
	ULONGLONG conversion6; \
	\
	FILETIME_ARITH(result6, inFileTime, -, FILETIME_1970); \
	FILETIME_2_SECONDS(conversion6, result6); \
	outTimeT = (time_t)conversion6; \
	TIME_END_MACRO
#define time_t_2_FILETIME(outFileTime, inTimeT) \
	TIME_BEGIN_MACRO \
	FILETIME conversion7; \
	\
	SECONDS_2_FILETIME(conversion7, inTimeT); \
	FILETIME_ARITH(outFileTime, conversion7, +, FILETIME_1970); \
	TIME_END_MACRO


/*
* Sometimes SYSTEMTIME needs to be handled as well.
*/
#define SYSTEMTIME_2_time_t(outTimeT, inSystemTime) \
	TIME_BEGIN_MACRO \
	FILETIME result8; \
	\
	SystemTimeToFileTime(&inSystemTime, &result8); \
	FILETIME_2_time_t(outTimeT, result8); \
	TIME_END_MACRO
#define time_t_2_SYSTEMTIME(outSystemTime, inTimeT) \
	TIME_BEGIN_MACRO \
	FILETIME conversion9; \
	\
	time_t_2_FILETIME(conversion9, inTimeT); \
	FileTimeToSystemTime(&conversion9, &outSystemTime); \
	TIME_END_MACRO
#define time_t_2_LOCALSYSTEMTIME(outSystemTime, inTimeT) \
	TIME_BEGIN_MACRO \
	FILETIME conversion10; \
	FILETIME localConversion10; \
	\
	time_t_2_FILETIME(conversion10, inTimeT); \
	FileTimeToLocalFileTime(&conversion10, &localConversion10); \
	FileTimeToSystemTime(&localConversion10, &outSystemTime); \
	TIME_END_MACRO

// 本地静态函数

static int a2w_buffer(LPCSTR inACPString, int inACPChars, LPWSTR outWideString, int inWideChars)
{
    int retval = 0;
    
    /*
    **  Start off by terminating the out argument if appropriate.
    */
    if(NULL != outWideString && 0 != inWideChars)
    {
        *outWideString = L'\0';
    }
    
    /*
    **  Sanity check arguments.
    */
    if(NULL != inACPString && 0 != inACPChars && (0 == inWideChars || NULL != outWideString))
    {
        /*
        **  Attempt the conversion.
        */
        retval = MultiByteToWideChar(
                                     CP_ACP,
                                     0,
                                     inACPString,
                                     inACPChars,
                                     outWideString,
                                     inWideChars
                                     );
    }
    
    return retval;
}

static LPWSTR a2w_malloc(LPCSTR inACPString, int inACPChars, int* outWideChars)
{
    LPWSTR retval = NULL;
    
    /*
    **  Initialize any out arguments.
    */
    if(NULL != outWideChars)
    {
        *outWideChars = 0;
    }
    
    /*
    **  Initialize the acp char length if requested.
    **  We do this here to avoid doing it twice in calls to a2w_buffer.
    */
    if(-1 == inACPChars)
    {
        if(NULL != inACPString)
        {
            /*
            **  Plus one so the terminating character is included.
            */
            inACPChars = (int)strlen(inACPString) + 1;
        }
        else
        {
            inACPChars = 0;
        }
    }
    
    /*
    **  Sanity check arguments.
    */
    if(NULL != inACPString && 0 != inACPChars)
    {
        int charsRequired = 0;
        
        /*
        **  Determine the size of buffer required for the conversion.
        */
        charsRequired = a2w_buffer(inACPString, inACPChars, NULL, 0);
        if(0 != charsRequired)
        {
            LPWSTR heapBuffer = NULL;
            
            heapBuffer = (LPWSTR)malloc((size_t)charsRequired * sizeof(WCHAR));
            if(NULL != heapBuffer)
            {
                int wideChars = 0;
                
                /*
                **  Real thing this time.
                */
                wideChars = a2w_buffer(inACPString, inACPChars, heapBuffer, charsRequired);
                if(0 != wideChars)
                {
                    retval = heapBuffer;
                    if(NULL != outWideChars)
                    {
                        *outWideChars = wideChars;
                    }
                }
                else
                {
                    /*
                    **  Something wrong.
                    **  Clean up.
                    */
                    free(heapBuffer);
                }
            }
        }
    }
    
    return retval;
}

static int w2a_buffer(LPCWSTR inWideString, int inWideChars, LPSTR outACPString, int inACPChars)
{
    int retval = 0;

    /*
    **  Start off by terminating the out argument if appropriate.
    */
    if(NULL != outACPString && 0 != inACPChars)
    {
        *outACPString = '\0';
    }

    /*
    **  Sanity check arguments.
    */
    if(NULL != inWideString && 0 != inWideChars && (0 == inACPChars || NULL != outACPString))
    {
        /*
        **  Attempt the conversion.
        */
        retval = WideCharToMultiByte(
                                     CP_ACP,
                                     0,
                                     inWideString,
                                     inWideChars,
                                     outACPString,
                                     inACPChars,
                                     NULL,
                                     NULL
                                     );
    }

    return retval;
}


static LPSTR w2a_malloc(LPCWSTR inWideString, int inWideChars, int* outACPChars)
{
    LPSTR retval = NULL;

    /*
    **  Initialize any out arguments.
    */
    if(NULL != outACPChars)
    {
        *outACPChars = 0;
    }

    /*
    **  Initialize the wide char length if requested.
    **  We do this here to avoid doing it twice in calls to w2a_buffer.
    */
    if(-1 == inWideChars)
    {
        if(NULL != inWideString)
        {
            /*
            **  Plus one so the terminating character is included.
            */
            inWideChars = (int)wcslen(inWideString) + 1;
        }
        else
        {
            inWideChars = 0;
        }
    }

    /*
    **  Sanity check arguments.
    */
    if(NULL != inWideString && 0 != inWideChars)
    {
        int charsRequired = 0;

        /*
        **  Determine the size of buffer required for the conversion.
        */
        charsRequired = w2a_buffer(inWideString, inWideChars, NULL, 0);
        if(0 != charsRequired)
        {
            LPSTR heapBuffer = NULL;

            heapBuffer = (LPSTR)malloc((size_t)charsRequired * sizeof(CHAR));
            if(NULL != heapBuffer)
            {
                int acpChars = 0;

                /*
                **  Real thing this time.
                */
                acpChars = w2a_buffer(inWideString, inWideChars, heapBuffer, charsRequired);
                if(0 != acpChars)
                {
                    retval = heapBuffer;
                    if(NULL != outACPChars)
                    {
                        *outACPChars = acpChars;
                    }
                }
                else
                {
                    /*
                    **  Something wrong.
                    **  Clean up.
                    */
                    free(heapBuffer);
                }
            }
        }
    }

    return retval;
}

extern "C" {
	
WCE_SHUNT_API int wce_printf(const char *, ...);

// string.h相关
char* wce_strerror(int inErrno)
{
#ifdef DEBUG
    wce_printf("wce_strerror called\n");
#endif
    return "Unknown Error";
}

// stdio.h相关

#define MAXFDS 200

typedef struct _fdtab_block
{
    int fd;
    FILE* file;
	char *fn;
} _fdtab_block;

static _fdtab_block _fdtab[MAXFDS];

void
_initfds()
{
    static int fdsinitialized = 0;
    
    if(fdsinitialized)
        return;
    
    for (int i = 0; i < MAXFDS; i++)
        _fdtab[i].fd = -1;
    
    fdsinitialized = 1;
}

int
_getnewfd()
{
    int i;
    
    for(i = 0; i < MAXFDS; i++)
    {
        if(_fdtab[i].fd == -1)
            return i;
    }
    
    return -1;
}

int wce_access(const char *path, int mode)
{
#ifdef DEBUG
        wce_printf("-- wce_access called\n");
#endif
    
    return 0;
}

void wce_rewind(FILE* inStream)
{
#ifdef DEBUG
        wce_printf("wce_rewind called\n");
#endif
    
    fseek(inStream, 0, SEEK_SET);
}

FILE* wce_fdopen(int fd, const char* inMode)
{
#ifdef DEBUG
        wce_printf("-- wce_fdopen called (mode is ignored!) \n");
#endif

    if(fd < 0 || fd >= MAXFDS || _fdtab[fd].fd == -1)
	    return 0;
    
    return _fdtab[fd].file;
}

void wce_perror(const char* inString)
{
#ifdef DEBUG
        wce_printf("wce_perror called\n");
#endif
    
    fprintf(stderr, "%s", inString);
}

int remove(const char* inPath)
{
#ifdef DEBUG
        wce_printf("wce_remove called on %s\n", inPath);
#endif
    
    int retval = -1;
    
    if(NULL != inPath)
    {
        WCHAR wPath[MAX_PATH];
        
        if(0 != a2w_buffer(inPath, -1, wPath, sizeof(wPath) / sizeof(WCHAR)))
        {
            if(FALSE != DeleteFileW(wPath))
            {
                retval = 0;
                
            }
        }
    }
    
    return retval;
}

int rename(const char *oldname, const  char *newname)
{
#ifdef DEBUG
        wce_printf("rename called on %s\n", oldname, newname);
#endif
    
    int retval = -1;
    
    if(NULL != oldname && NULL != newname)
    {
        WCHAR wPath1[MAX_PATH];
		WCHAR wPath2[MAX_PATH];
        
        if (0 != a2w_buffer(oldname, -1, wPath1, sizeof(wPath1) / sizeof(WCHAR)) ||
			0 != a2w_buffer(newname, -1, wPath2, sizeof(wPath2) / sizeof(WCHAR)))
        {
            if(FALSE != MoveFileW(wPath1, wPath2))
            {
                retval = 0;
            }
        }
    }
    
    return retval;
}

char* wce_getcwd(char* buff, size_t size)
{
#ifdef DEBUG
    wce_printf("wce_getcwd called.\n");
#endif

	int i;
    WCHAR dir[MAX_PATH];
    GetModuleFileName(GetModuleHandle (NULL), dir, MAX_PATH);
    for (i = _tcslen(dir); i && dir[i] != TEXT('\\'); i--) {}
    dir[i + 1] = WCHAR('\0');
    
    w2a_buffer(dir, -1, buff, size);
    
    return buff;
}

int wce_printf(const char * format, ...)
{
    return 0;
}

static void mode2binstr(int mode, char* buffer)
{
	if ((mode & O_RDWR) || (mode & O_WRONLY) || (mode & O_APPEND))  // write only == read|write
    {
        if (mode & O_CREAT)
        {
			if (mode & O_TEXT)
				strcpy(buffer, "wt+");
			else
				strcpy(buffer, "wb+");
        }
        else if (mode & O_APPEND)
        {
			if (mode & O_TEXT)
				strcpy(buffer, "at+");
			else
				strcpy(buffer, "ab+");
        }
        else if (mode & O_TRUNC)
        {
			if (mode & O_TEXT)
				strcpy(buffer, "wt+");
			else
				strcpy(buffer, "wb+");
        }
        else if (mode == O_RDWR)
        {
			if (mode & O_TEXT)
				strcpy(buffer, "rt+");
			else
				strcpy(buffer, "rb+");
        }
    }
    else
    {
		if (mode & O_TEXT)
			strcpy(buffer, "rt");
		else
			strcpy(buffer, "rb");
    }
}

int open(const char *pathname, int mode, ...)
{
#ifdef DEBUG
        wce_printf("open called\n");
#endif
    
    _initfds();
        
    char modestr[10];
    *modestr = '\0';
    
    mode2binstr(mode, modestr);
    if (*modestr == '\0')
        return -1;
    
    FILE* file = fopen(pathname, modestr);
    
    int fd = -1;
    
    if (file)
    {
        fd = _getnewfd();
        
        _fdtab[fd].fd = fd;
        _fdtab[fd].file = file;
		_fdtab[fd].fn = _strdup(pathname);
        
        fflush(file);
        fread(NULL, 0, 0, file);
    }
    
    return fd;
}

int close(int fd)
{
#ifdef DEBUG
        wce_printf("wce_close called\n");
#endif
    
    if(fd < 0 || fd >= MAXFDS || _fdtab[fd].fd == -1)
	    return -1;
    
    
    fclose(_fdtab[fd].file);
    _fdtab[fd].fd = -1;
	free(_fdtab[fd].fn);
    
    return 0;
}

size_t read(int fd, void* buffer, size_t count)
{
#ifdef DEBUG
        wce_printf("wce_read called\n");
#endif
    
    if(fd < 0 || fd >= MAXFDS || _fdtab[fd].fd == -1)
	    return -1;
    
    size_t num = fread(buffer, 1, count, _fdtab[fd].file);
    
    if (ferror(_fdtab[fd].file))
        return -1;
    
    return num;
}


size_t write(int fd, const void* buffer, size_t count)
{
#ifdef DEBUG
        wce_printf("wce_write called\n");
#endif
    
    if(fd < 0 || fd >= MAXFDS || _fdtab[fd].fd == -1)
	    return -1;
    
    size_t num = fwrite(buffer, 1, count, _fdtab[fd].file);
    if (ferror(_fdtab[fd].file))
        return -1;
    
    return num;
}


int wce_unlink(const char *pathname)
{
#ifdef DEBUG
        wce_printf("wce_unlink called\n");
#endif
    return remove(pathname);
}


int wce_lseek(int fd, int offset, int whence)
{
#ifdef DEBUG
        wce_printf("wce_lseek called\n");
#endif
    
    
    if(fd < 0 || fd >= MAXFDS || _fdtab[fd].fd == -1)
	return -1;
    
    int newpos = -1;
    int error = fseek(_fdtab[fd].file, offset, whence);
    
    if (!error)
        newpos = ftell(_fdtab[fd].file);
    return newpos;
}

int wce_setvbuf(FILE *stream, char *buffer, int mode, size_t size)
{
	return 0;
}

int wce_fileno(FILE* f)
{
#ifdef DEBUG
        wce_printf("wce_fileno called\n");
#endif
    
    // Windows SDK is broken.  _fileno should return an int, but for whatever god forsaken reason, CE returns void*.
    int fd;
    
    for(fd = 0; fd < MAXFDS; fd++)
    {
        if(_fdtab[fd].file == f)
		{
            return _fdtab[fd].fd;
		}
    }
    return (int) fileno(f);
}

int wce_fstat(int fd, struct stat *sb)
{
	if(fd < 0 || fd >= MAXFDS || _fdtab[fd].fd == -1)
	    return -1;

	if (_fdtab[fd].fn)
	{
		extern int wce_stat(const char* inPath, struct stat* outStats);
		return wce_stat(_fdtab[fd].fn, sb);
	}

	return -1;
}

// stdlib.h相关

char *wce_fullpath(char *absPath, const char *relPath, size_t maxLength)
{
#ifdef LOG_CALLS
#ifdef DEBUG
	wce_printf("wce_fullpath called\n");
#endif
#endif

	if (relPath[0] != '\\') 
	{
		int i;
		WCHAR dir[MAX_PATH];
		GetModuleFileName(GetModuleHandle (NULL), dir, MAX_PATH);
		for (i = _tcslen(dir); i && dir[i] != TEXT('\\'); i--) {}
		dir[i + 1] = TCHAR('\0');

		w2a_buffer(dir, -1, absPath, maxLength);
	}
	strcat(absPath, relPath);

	return absPath;
}

void wce_splitpath(const char* inPath, char* outDrive, char* outDir, char* outFname, char* outExt)
{
#ifdef LOG_CALLS
#ifdef DEBUG
	wce_printf("wce_splitpath called\n");
#endif
#endif
	if(NULL != outDrive)
	{
		*outDrive = '\0';
	}
	if(NULL != outDir)
	{
		*outDir = '\0';
	}
	if(NULL != outFname)
	{
		*outFname = '\0';
	}
	if(NULL != outExt)
	{
		*outExt = '\0';
	}

	if(NULL != inPath && '\0' != *inPath)
	{
		char* dup = (char*) malloc(strlen(inPath));
		if(NULL != dup)
		{
			strcpy(dup, inPath);
			/*
			**  Change all forward slashes to back.
			*/
			char* convert = dup;
			do
			{
				if('/' == *convert)
				{
					*convert = '\\';
				}
				convert++;
			}
			while(*convert);

			/*
			**  Find last slash first.
			*/
			char* slash = strrchr(dup, '\\');

			/*
			**  Find extension, must be after any slash.
			*/
			char* ext = NULL;
			if(NULL == slash)
			{
				ext = strchr(dup, '.');
			}
			else
			{
				ext = strchr(slash, '.');
			}

			/*
			**  Reap extension.
			*/
			if(NULL != ext)
			{
				if(NULL != outExt)
				{
					strncpy(outExt, ext, _MAX_EXT);
				}

				*ext = '\0';
			}

			/*
			**  Reap filename.
			*/
			char* fname = NULL;
			if(NULL == slash)
			{
				fname = dup;
			}
			else
			{
				fname = slash + 1;
			}

			if(NULL != outFname)
			{
				strncpy(outFname, fname, _MAX_FNAME);
			}

			*fname = '\0';

			/*
			**  Reap directory.
			*/
			if(NULL != slash && NULL != outDir)
			{
				strncpy(outDir, dup, _MAX_DIR);
			}

			free(dup);
		}
	}
}

void wce_makepath(char* outPath, const char* inDrive, const char* inDir, const char* inFname, const char* inExt)
{
#ifdef LOG_CALLS
#ifdef DEBUG
	wce_printf("wce_makepath called\n");
#endif
#endif
	if(NULL != outPath)
	{
		int dirLen = 0;
		if(NULL != inDir)
		{
			dirLen = strlen(inDir);
			if(dirLen)
			{
				dirLen--;
			}
		}
		snprintf(outPath, _MAX_PATH, "%s%s%s%s%s",
			(NULL != inDir) ? inDir : "",
			(NULL != inDir && '\\' != inDir[dirLen] && '/' != inDir[dirLen]) ? "\\" : "",
			(NULL != inFname) ? inFname : "",
			(NULL != inExt && '.' != inExt[0]) ? "." : "",
			(NULL != inExt) ? inExt : ""
			);
	}
}

void *bsearch(const void *key, const void *base, size_t high,
			  size_t size, int (*compar)(const void *, const void *))
{
	register char *p;
	size_t low;
	size_t mid;
	int r;

	if (size > 0) {				/* TODO: change this to an assert?? */
		low = 0;
		while (low < high) {
			mid = low + ((high - low) >> 1); /* Avoid possible overflow here. */
			p = ((char *)base) + mid * size; /* Could overflow here... */
			r = (*compar)(key, p); /* but that's an application problem! */
			if (r > 0) {
				low = mid + 1;
			} else if (r < 0) {
				high = mid;
			} else {
				return p;
			}
		}
	}
	return NULL;
}

errno_t wce_rand_s(unsigned int *r)
{
	static bool srand_called = false;
	if (!srand_called)
		srand(time(0));
	*r = rand();
	return 0;
}

// process.h相关

void wce_abort(void)
{
#ifdef DEBUG
    wce_printf("wce_abort called\n");
#endif

#if defined(DEBUG)
    DebugBreak();
#endif
    TerminateProcess(GetCurrentProcess(), 3);
}

// 环境变量读写
typedef struct _ENV
{
	struct _ENV *next;
	char *name;
	char *value;
} ENV;
static ENV *s_wceEnvs = NULL;
CRITICAL_SECTION s_fMutex;
class EnvDeleter
{
public:
	EnvDeleter()
	{
		::InitializeCriticalSection(&s_fMutex);
		s_wceEnvs = NULL;
		wce_setenv("HOME", "\\");
	}
	~EnvDeleter()
	{
		::EnterCriticalSection(&s_fMutex);
		ENV *env = s_wceEnvs;
		while (env)
		{
			ENV *de = env;
			env = env->next;
			free(env);
		}
		s_wceEnvs = NULL;
		::LeaveCriticalSection(&s_fMutex);
		::DeleteCriticalSection(&s_fMutex);
	}
};
static EnvDeleter s_envDeleter;

char* wce_getenv(const char* inName)
{
#ifdef DEBUG
    wce_printf("wce_getenv called (%s)\n", inName);
#endif

	::EnterCriticalSection(&s_fMutex);

	ENV *env = s_wceEnvs;
	while (env)
	{
		if (stricmp(inName, env->name) == 0)
		{
			::LeaveCriticalSection(&s_fMutex);
			return env->value;
		}
		env = env->next;
	}

	::LeaveCriticalSection(&s_fMutex);

    return NULL;
}

int wce_setenv(const char *inName, const char *inValue) 
{
#ifdef DEBUG
    wce_printf("wce_putenv called\n");
#endif

	::EnterCriticalSection(&s_fMutex);

	ENV *env = s_wceEnvs, *prev = NULL;
	while (env)
	{
		if (stricmp(inName, env->name) == 0)
			break;
		prev = env;
		env = env->next;
	}
	if (env)
	{
		if (prev)
			prev->next = env->next;
		else
			s_wceEnvs = env->next;
		free(env);
	}

	int lenE = ALIGN_INT(sizeof(ENV));
	int lenN = ALIGN_INT(strlen(inName) + 1), lenV = ALIGN_INT(strlen(inValue) + 1);
	env = (ENV *)malloc(lenE + lenN + lenV);
	if (env)
	{
		env->name = (char *)env + lenE;
		env->value = env->name + lenN;
		strcpy(env->name, inName);
		strcpy(env->value, inValue);

		env->next = s_wceEnvs;
		s_wceEnvs = env;
	}

	::LeaveCriticalSection(&s_fMutex);
    return 0;
}

// signal.h相关
static _sigsig sigArray[_SIGCOUNT];

static void defaultSighandler(int inSignal)
{
    wce_abort();
}

int wce_raise(int inSignal)
{
#ifdef DEBUG
    wce_printf("wce_raise called\n");
#endif

    void (*handler)(int inSignal) = defaultSighandler;

    if(inSignal >= 0 && inSignal < _SIGCOUNT)
    {
        if(NULL != sigArray[inSignal])
        {
            handler = sigArray[inSignal];
        }
    }

    handler(inSignal);
    return 0;
}

_sigsig wce_signal(int inSignal, _sigsig inFunc)
{
#ifdef DEBUG
    wce_printf("wce_signal called\n");
#endif

    void (*retval)(int inSignal) = defaultSighandler;

    if(inSignal >= 0 && inSignal < _SIGCOUNT)
    {
        if(NULL != sigArray[inSignal])
        {
            retval = sigArray[inSignal];
        }
        sigArray[inSignal] = inFunc;
    }

    return retval;
}

// time.h相关
#define MAX_TM_SIZE 16
static struct tm tmStorage[MAX_TM_SIZE];
static int tmSlot = 0;

static struct tm *tm_getslot()
{
	struct tm *tmBuf = &tmStorage[tmSlot++];
	if (tmSlot >= MAX_TM_SIZE)
		tmSlot = 0;
	return tmBuf;
}

time_t __cdecl time(time_t *t32)
{
	__time64_t t64;
	time_t r = (time_t)_time64(&t64);
	if (t32)
		*t32 = (time_t)t64;
	return r;
}

struct tm * __cdecl gmtime(const time_t *t32)
{
	__time64_t t64 = (__time64_t)t32;
	struct tm *tmBuf = tm_getslot();
	_gmtime64_s(tmBuf, &t64);
	return tmBuf;
}

time_t __cdecl mktime(struct tm *tmBuf)
{
	return (time_t)_mktime64(tmBuf);
}

errno_t localtime_s(struct tm * _Tm, const time_t *_Time)
{
	__time64_t t64 = *_Time;
	return _localtime64_s(_Tm, &t64);
}

#if 0
struct tm * __cdecl localtime(const time_t *_Time)
{
	static struct tm stms[8];
	static int stmw = 0;
	__time64_t t64 = *_Time;
	struct tm *r = &stms[stmw++];
	stmw = stmw >= 8 ? 0 : (stmw + 1);
	_localtime64_s(r, &t64);
	return r;
}
#endif

void _ftime(struct wce__timeb * _Time)
{
	{
		SYSTEMTIME sysTime;
		union {
			__int64 ns100; /*time since 1 Jan 1601 in 100ns units */
			FILETIME ft;
		} _now;

		GetSystemTime(&sysTime);
		SystemTimeToFileTime(&sysTime, &_now.ft);

		_Time->time = (long) ((_now.ns100 - 116444736000000000) / 10000000);
		_Time->millitm = (short)((long) ((_now.ns100 / 10) % 1000000) / 1000);
	}
	{
		long tzone = _timezone;
		_Time->timezone = (short)tzone;
	}
	_Time->dstflag = 0;
}

size_t __cdecl strftime(char *buf, size_t maxsize, const char *fmt, const struct tm *_Tm)
{
	size_t size = 0;
	LPWSTR wideFmt = NULL;
	wideFmt = a2w_malloc(fmt, -1, NULL);
	if(NULL != wideFmt)
	{
		WCHAR *wideBuf = (WCHAR *)malloc(maxsize * sizeof(WCHAR));
		if (wideBuf)
		{
			size = wcsftime(wideBuf, maxsize, wideFmt, _Tm);
			w2a_buffer(wideBuf, size, buf, maxsize);
			free(wideBuf);
		}
		free(wideFmt);
	}
	return size;
}

// for ThreadingPthreads.cpp
int gettimeofday(struct timeval *p, struct timezone *tz)
{
	SYSTEMTIME sysTime;
	union {
		__int64 ns100; /*time since 1 Jan 1601 in 100ns units */
		FILETIME ft;
	} _now;

	GetSystemTime(&sysTime);
	SystemTimeToFileTime(&sysTime, &_now.ft);

	p->tv_usec = (long) ((_now.ns100 / 10) % 1000000);
	p->tv_sec  = (long) ((_now.ns100 - 116444736000000000) / 10000000);
	return 0;
}

int settimeofday(const struct timeval *p, const struct timezone *tz)
{
	return 0;
}

// stat相关

int wce_stat(const char* inPath, struct stat* outStats)
{
#ifdef DEBUG
	wce_printf("wce_stat called\n");
#endif

	int retval = -1;

	if(NULL != outStats)
	{
		memset(outStats, 0, sizeof(struct stat));

		if(NULL != inPath)
		{
			WCHAR wPath[MAX_PATH];

			int convRes = a2w_buffer(inPath, -1, wPath, sizeof(wPath) / sizeof(WCHAR));
			if(0 != convRes)
			{
				HANDLE readHandle;
				WIN32_FIND_DATA findData;
				readHandle = FindFirstFileW(wPath, &findData); 

				if (readHandle != INVALID_HANDLE_VALUE && readHandle != NULL)
				{
					retval = 0;
					outStats->st_size = findData.nFileSizeLow;

					if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						outStats->st_mode = _S_IFDIR;
					}
					else
					{
						outStats->st_mode = _S_IFREG;
					}

					FILETIME_2_time_t(outStats->st_ctime, findData.ftCreationTime);
					FILETIME_2_time_t(outStats->st_atime, findData.ftLastAccessTime);
					FILETIME_2_time_t(outStats->st_mtime, findData.ftLastWriteTime);

					outStats->st_mode |= _S_IREAD;
					if(0 == (FILE_ATTRIBUTE_READONLY & findData.dwFileAttributes))
					{
						outStats->st_mode |= _S_IWRITE;
					}
					if(FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes)
					{
						outStats->st_mode |= _S_IFDIR;
					}
					else
					{
						outStats->st_mode |= _S_IFREG;
					}
					FindClose(readHandle);
				}
				else
				{	
					// From Errno.h
					errno = ENOENT;
				}
			}
		}
	}

	return retval;
}

int wce_wstat(const WCHAR* inPath, struct stat* outStats)
{
#ifdef DEBUG
	wce_printf("wce_stat called\n");
#endif

	int retval = -1;

	if(NULL != outStats)
	{
		memset(outStats, 0, sizeof(struct stat));

		if(NULL != inPath)
		{
			HANDLE readHandle;
			WIN32_FIND_DATA findData;
			readHandle = FindFirstFileW(inPath, &findData); 

			if (readHandle != INVALID_HANDLE_VALUE && readHandle != NULL)
			{
				retval = 0;
				outStats->st_size = findData.nFileSizeLow;

				if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					outStats->st_mode = _S_IFDIR;
				}
				else
				{
					outStats->st_mode = _S_IFREG;
				}

				FILETIME_2_time_t(outStats->st_ctime, findData.ftCreationTime);
				FILETIME_2_time_t(outStats->st_atime, findData.ftLastAccessTime);
				FILETIME_2_time_t(outStats->st_mtime, findData.ftLastWriteTime);

				outStats->st_mode |= _S_IREAD;
				if(0 == (FILE_ATTRIBUTE_READONLY & findData.dwFileAttributes))
				{
					outStats->st_mode |= _S_IWRITE;
				}
				if(FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes)
				{
					outStats->st_mode |= _S_IFDIR;
				}
				else
				{
					outStats->st_mode |= _S_IFREG;
				}
			}
		}
	}

	return retval;
}

// direct.h相关

int mkdir(const char* inDirname)
{
#ifdef DEBUG
	wce_printf("mkdir called\n");
#endif

	int retval = -1;

	if(NULL != inDirname)
	{
		WCHAR wDirname[MAX_PATH];

		if(0 != a2w_buffer(inDirname, -1, wDirname, sizeof(wDirname) / sizeof(unsigned short)))
		{
			if(FALSE != CreateDirectoryW(wDirname, NULL))
			{
				retval = 0;
			}
		}
	}

	return retval;
}

int rmdir(const char* inDirname)
{
#ifdef DEBUG
	wce_printf("rmdir called (%s)\n", inDirname);
#endif

	int retval = -1;

	if(NULL != inDirname)
	{
		WCHAR wDirname[MAX_PATH];

		if(0 != a2w_buffer(inDirname, -1, wDirname, sizeof(wDirname) / sizeof(unsigned short)))
		{
			if(FALSE != RemoveDirectoryW(wDirname))
			{
				retval = 0;
			}
		}
	}

	return retval;
}

int chdir(const char* inDirname)
{
#ifdef DEBUG
	wce_printf("chdir called (%s)\n", inDirname);
#endif

	int retval = -1;

	SetLastError(ERROR_NOT_SUPPORTED);

	return retval;
}

// From dirent.cpp
struct _DIR
{
	HANDLE handle;
	WIN32_FIND_DATAW data;
	struct dirent e;
};

DIR *wce_opendir(const char *path)
{
	WCHAR wstr[MAX_PATH];
	if (a2w_buffer(path, -1, wstr, MAX_PATH))
	{
		DIR *dir = (DIR *)malloc(sizeof(DIR));
		if (dir)
		{
			if (INVALID_HANDLE_VALUE != (dir->handle = FindFirstFileW(wstr, &dir->data)))
			{
				dir->e.d_name = NULL;
				return dir;
			}
			free(dir);
		}
	}
	return NULL;
}

struct dirent *wce_readdir(DIR *d)
{
	if (FindNextFileW(d->handle, &d->data))
	{
		if (d->e.d_name)
			free(d->e.d_name), d->e.d_name = NULL;
		d->e.d_name = w2a_malloc(d->data.cFileName, -1, NULL);
		if (d->e.d_name)
			return &d->e;
	}
	return NULL;
}

int wce_closedir(DIR *d)
{
	FindClose(d->handle);
	if (d->e.d_name)
		free(d->e.d_name);
	free(d);
	return 0;
}

// WIN32相关

LONG wce_GetMessageTime(void)
{
#ifdef DEBUG
	wce_printf("wce_GetMessageTime called\n");
#endif
	// Close enough guess?
	return GetTickCount();
}

BOOL wce_InitializeSecurityDescriptor(PSECURITY_DESCRIPTOR, DWORD)
{
#ifdef DEBUG
	wce_printf("wce_InitializeSecurityDescriptor called\n");
#endif
	return FALSE;
}

BOOL wce_SetSecurityDescriptorDacl(PSECURITY_DESCRIPTOR, BOOL, PACL, BOOL)
{
#ifdef DEBUG
	wce_printf("wce_SetSecurityDescriptorDacl called\n");
#endif
	return FALSE;
}

LCID wce_GetThreadLocale(void)
{
#ifdef DEBUG
	wce_printf("wce_GetThreadLocale called\n");
#endif
	return GetSystemDefaultLCID();
}

static struct lconv s_locale_conv =
{
	".",   /* decimal_point */
	",",   /* thousands_sep */
	"333", /* grouping */
	"$",   /* int_curr_symbol */
	"$",   /* currency_symbol */
	"",    /* mon_decimal_point */
	"",    /* mon_thousands_sep */
	"",    /* mon_grouping */
	"+",   /* positive_sign */
	"-",   /* negative_sign */
	'2',   /* int_frac_digits */
	'2',   /* frac_digits */
	1,     /* p_cs_precedes */
	1,     /* p_sep_by_space */
	1,     /* n_cs_precedes */
	1,     /* n_sep_by_space */
	1,     /* p_sign_posn */
	1,     /* n_sign_posn */
};

struct lconv *localeconv(void)
{
#ifdef DEBUG
	wce_printf("wce_localeconv called\n");
#endif
	return &s_locale_conv;
}

char *setlocale(int, const char *)
{
	return NULL;
}

//-----------------------------------------------------------------------------
// Fake implementation of GetAncestor. If API not present, this will try to
// do what the real implementation does.
//-----------------------------------------------------------------------------
HWND WINAPI wce_GetAncestor(HWND hwnd, UINT gaFlags)
{
	HWND	hwndParent;
	HWND	hwndDesktop;
	DWORD   dwStyle;

	// HERE IS THE FAKE IMPLEMENTATION
	if (!IsWindow(hwnd))
		return(NULL);

	hwndDesktop = GetDesktopWindow();
	if (hwnd == hwndDesktop)
		return(NULL);
	dwStyle = GetWindowLong (hwnd,GWL_STYLE);

	switch (gaFlags)
	{
	case GA_PARENT:
		if (dwStyle & WS_CHILD)
			hwndParent = GetParent(hwnd);
		else
			hwndParent = GetWindow (hwnd,GW_OWNER);

		if (hwndParent == NULL)
			hwndParent = hwnd;
		break;

	case GA_ROOT:
		if (dwStyle & WS_CHILD)
			hwndParent = GetParent(hwnd);
		else
			hwndParent = GetWindow (hwnd,GW_OWNER);

		while (hwndParent != hwndDesktop &&
			hwndParent != NULL)
		{
			hwnd = hwndParent;
			dwStyle = GetWindowLong(hwnd,GWL_STYLE);
			if (dwStyle & WS_CHILD)
				hwndParent = GetParent(hwnd);
			else
				hwndParent = GetWindow (hwnd,GW_OWNER);
		}
		break;

	case GA_ROOTOWNER:
		while (hwndParent = GetParent(hwnd))
			hwnd = hwndParent;
		break;
	}

	return(hwndParent);
}

int wce_SetStretchBltMode(HDC inDC, int inStretchMode)
{
#ifdef DEBUG
	wce_printf("-- wce_SetStretchBltMode called\n");
#endif

	int retval = 0;

	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);

	return retval;
}

DWORD wce_GetFontData(HDC inDC, DWORD inTable, DWORD inOffset, LPVOID outBuffer, DWORD inData)
{
#ifdef DEBUG
	wce_printf("-- wce_GetFontData called\n");
#endif

	DWORD retval = GDI_ERROR;
	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);

	return retval;
}

UINT wce_GetOutlineTextMetrics(HDC inDC, UINT inData, void* outOTM)
{
#ifdef DEBUG
	wce_printf("-- wce_GetOutlineTextMetrics called.\n");
#endif

	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	return 0;
}

BOOL wce_GdiFlush(void)
{
	return TRUE;
}

void wce_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
#ifdef DEBUG
	wce_printf("wce_GetSystemTimeAsFileTime called\n");
#endif

	SYSTEMTIME st;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st,lpSystemTimeAsFileTime);
}

int wce_FrameRect(HDC inDC, CONST RECT *inRect, HBRUSH inBrush)
{
#ifdef DEBUG
	wce_printf("wce_FrameRect called\n");
#endif

	HBRUSH oldBrush = (HBRUSH)SelectObject(inDC, inBrush);
	RECT myRect = *inRect;
	InflateRect(&myRect, 1, 1); // The width and height of
	// the border are always one
	// logical unit.

	// 1  ---->   2
	//            
	// ^          |
	// |          v
	//
	// 4  ---->   3 

	MoveToEx(inDC, myRect.left, myRect.top, (LPPOINT) NULL); 

	// 1 -> 2
	LineTo(inDC, myRect.right, myRect.top); 

	// 2 -> 3
	LineTo(inDC, myRect.right, myRect.bottom); 

	// 3 -> 4
	LineTo(inDC, myRect.left, myRect.bottom);

	// 3 -> 4
	LineTo(inDC, myRect.left, myRect.top); 

	SelectObject(inDC, oldBrush);

	return 1;
}

// WIN32A相关
VOID wce_OutputDebugStringA(LPCSTR inOutputString)
{
#ifdef DEBUG
	wce_printf("wce_OutputDebugStringA called\n");
#endif

	LPWSTR wideStr = NULL;

	wideStr = a2w_malloc(inOutputString, -1, NULL);
	if(NULL != wideStr)
	{
		OutputDebugStringW(wideStr);
		free(wideStr);
	}
}

DWORD wce_GetModuleFileNameA(HMODULE hModule, LPSTR lpFilename, DWORD nSize)
{
#ifdef DEBUG
	wce_printf("wce_GetModuleFileNameA called\n");
#endif

	WCHAR wideStr[MAX_PATH];

	DWORD result = w2a_buffer(wideStr, 
		GetModuleFileNameW(hModule, 
		wideStr, 
		sizeof(wideStr)), 
		lpFilename, nSize);
	lpFilename[result] = '\0';

	return result;
}

HANDLE wce_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
#ifdef DEBUG
	wce_printf("wce_CreateFileA called (%s)\n", lpFileName);
#endif

	HANDLE retval = INVALID_HANDLE_VALUE;
	WCHAR wideStr[MAX_PATH];

	if(a2w_buffer(lpFileName, -1, wideStr, MAX_PATH))
	{
		retval = CreateFileW(wideStr, 
			dwDesiredAccess, 
			dwShareMode, 
			lpSecurityAttributes, 
			dwCreationDisposition, 
			dwFlagsAndAttributes, 
			hTemplateFile);
	}
	return retval;
}

HANDLE wce_CreateFileMappingA(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName)
{
#ifdef DEBUG
	wce_printf("wce_CreateFileMappingA called\n");
#endif

	HANDLE h = NULL;

	LPWSTR wString = a2w_malloc(lpName, -1, NULL);
	if (lpName == NULL || wString) {
		h = CreateFileMappingW(hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, wString);
		if (wString) free(wString);
	}
	return h;
}

LONG wce_RegOpenKeyExA(HKEY inKey, LPCSTR inSubKey, DWORD inOptions, REGSAM inSAM, PHKEY outResult)
{
#ifdef DEBUG
	wce_printf("wce_RegOpenKeyExA called\n");
#endif

	LONG retval = ERROR_GEN_FAILURE;

	LPWSTR wSubKey = a2w_malloc(inSubKey, -1, NULL);
	if(NULL != wSubKey)
	{
		retval = RegOpenKeyEx(inKey, wSubKey, inOptions, inSAM, outResult);
		free(wSubKey);
	}
	return retval;
}

LONG wce_RegQueryValueExA(HKEY inKey, LPCSTR inValueName, LPDWORD inReserved, LPDWORD outType, LPBYTE inoutBData, LPDWORD inoutDData)
{
#ifdef DEBUG
	wce_printf("wce_RegQueryValueExA called\n");
#endif

	LONG retval = ERROR_GEN_FAILURE;

	LPWSTR wName = a2w_malloc(inValueName, -1, NULL);
	if(NULL != wName)
	{
		DWORD tempSize = *inoutDData * sizeof(WCHAR); /* in bytes */
		LPWSTR tempData = (LPWSTR)malloc(tempSize);
		if(NULL != tempData)
		{
			retval = RegQueryValueEx(inKey, wName, inReserved, outType, (LPBYTE)tempData, &tempSize);

			/*
			**  Convert to ANSI if a string....
			*/
			if(ERROR_SUCCESS == retval && (
				REG_EXPAND_SZ == *outType ||
				REG_MULTI_SZ == *outType ||
				REG_SZ == *outType
				))
			{
				*inoutDData = (DWORD)w2a_buffer(tempData, tempSize / sizeof(WCHAR), (LPSTR)inoutBData, *inoutDData);
			}
			else
			{
				memcpy(inoutBData, tempData, tempSize);
				*inoutDData = tempSize;
			}

			free(tempData);
		}

		free(wName);
	}

	return retval;
}

LONG wce_RegSetValueExA(HKEY hKey, const char *valname, DWORD dwReserved, DWORD dwType, LPBYTE lpData, DWORD dwSize)
{
#ifdef DEBUG
	wce_printf("wce_RegSetValueExA called\n");
#endif

	WCHAR valnamew[256];
	LONG res;

	LPBYTE lpDataNew = lpData;
	DWORD dwDataSize = dwSize;

	MultiByteToWideChar(CP_ACP, 0, valname, -1, valnamew, sizeof(valnamew) / sizeof(WCHAR));

	if(dwType == REG_SZ || dwType == REG_EXPAND_SZ)
	{
		dwDataSize = dwSize * 2;
		lpDataNew = (LPBYTE) malloc(dwDataSize);

		MultiByteToWideChar(CP_ACP, 0, (const char*) lpData, -1, (WCHAR *)lpDataNew, dwDataSize);
	}

	res = RegSetValueExW(hKey, valnamew, dwReserved, dwType, lpDataNew, dwDataSize);

	free(lpDataNew);

	return res;

}

LONG wce_RegCreateKeyExA(HKEY hKey, const char *subkey, DWORD dwRes, LPSTR lpszClass, DWORD ulOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES sec_att, PHKEY phkResult, DWORD *lpdwDisp)
{
#ifdef DEBUG
	wce_printf("wce_RegCreateKeyExA called\n");
#endif

	LPWSTR wName = a2w_malloc(subkey, -1, NULL);

	LONG res = RegCreateKeyExW(hKey, wName, dwRes, NULL, ulOptions, samDesired, NULL, phkResult, lpdwDisp);

	free(wName);
	return res;
}

LONG wce_RegDeleteValueA(HKEY hKey, LPCSTR lpValueName)
{
#ifdef DEBUG
	wce_printf("wce_RegDeleteValueA called\n");
#endif

	LPWSTR wName = a2w_malloc(lpValueName, -1, NULL);
	LONG res = RegDeleteKeyW(hKey, wName);

	free(wName);
	return res;
}

LONG wce_RegEnumValueA(HKEY hKey, DWORD dwIndex, LPSTR lpValueName, LPDWORD lpcbValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
#ifdef DEBUG
	wce_printf("wce_RegDeleteValueA called\n");
#endif

	LPWSTR wName = a2w_malloc(lpValueName, -1, NULL);
	LONG res = RegEnumValueW(hKey, dwIndex, wName, lpcbValueName, lpReserved, lpType, lpData, lpcbData);

	free(wName);
	return res;
}

BOOL wce_GetVersionExA(LPOSVERSIONINFOA lpv)
{
#ifdef DEBUG
	wce_printf("wce_GetVersionExA called\n");
#endif

	OSVERSIONINFOW vw;
	vw.dwOSVersionInfoSize = sizeof(vw);

	GetVersionExW(&vw);

	memcpy(lpv, &vw, 5 * sizeof(DWORD));

	w2a_buffer(vw.szCSDVersion, -1, lpv->szCSDVersion, sizeof(lpv->szCSDVersion));    
	return TRUE;
}

BOOL wce_GetTextMetricsA(HDC hdc, LPTEXTMETRICA lptma)
{
#ifdef DEBUG
	wce_printf("wce_GetTextMetricsA called\n");
#endif

	if (!lptma)
		return 0;

	TEXTMETRICW tmw;
	BOOL res;

	res = GetTextMetricsW(hdc, &tmw);

	if (res==0)
		return res;

	lptma->tmHeight = tmw.tmHeight;
	lptma->tmAscent = tmw.tmAscent;
	lptma->tmDescent = tmw.tmDescent;
	lptma->tmInternalLeading = tmw.tmInternalLeading;
	lptma->tmExternalLeading = tmw.tmExternalLeading;
	lptma->tmAveCharWidth = tmw.tmAveCharWidth;
	lptma->tmMaxCharWidth = tmw.tmMaxCharWidth;
	lptma->tmWeight = tmw.tmWeight;
	lptma->tmOverhang = tmw.tmOverhang;
	lptma->tmDigitizedAspectX = tmw.tmDigitizedAspectX;
	lptma->tmDigitizedAspectY = tmw.tmDigitizedAspectY;
	lptma->tmItalic = tmw.tmItalic;
	lptma->tmUnderlined = tmw.tmUnderlined;
	lptma->tmStruckOut = tmw.tmStruckOut;
	lptma->tmPitchAndFamily = tmw.tmPitchAndFamily;
	lptma->tmCharSet = tmw.tmCharSet;

	w2a_buffer(&tmw.tmFirstChar, 1, &lptma->tmFirstChar, 1);
	w2a_buffer(&tmw.tmDefaultChar, 1, &lptma->tmDefaultChar, 1);
	w2a_buffer(&tmw.tmBreakChar, 1, &lptma->tmBreakChar, 1);

	return res;
}

HINSTANCE wce_LoadLibraryA(LPCSTR lpLibFileName)
{
#ifdef DEBUG
	wce_printf("wce_LoadLibraryA called (%s)\n", lpLibFileName);
#endif

	HINSTANCE retval = NULL;

	LPWSTR wPath = a2w_malloc(lpLibFileName, -1, NULL);

	if(wPath) {
		retval = LoadLibraryW(wPath);
	}

#ifdef DEBUG
	if (!retval) {
		DWORD error = GetLastError();
		wce_printf("LoadLibraryA failure (14==OOM)! %d\n", error);

		if (error == 14)
			MessageBoxW(NULL, L"Failed to Load Library. Out Of Memory.", wPath, 0);
	}
#endif

	if (wPath)
		free(wPath);
	return retval;
}

HMODULE wce_GetModuleHandleA(const char *lpName)
{
#ifdef DEBUG
	wce_printf("wce_GetModuleHandleA called (%s)\n", lpName);
#endif

	if (lpName == NULL)
		return GetModuleHandleW(NULL);

	WCHAR wideStr[MAX_PATH];
	if(a2w_buffer(lpName, -1, wideStr, MAX_PATH))
		return GetModuleHandleW(wideStr);

	return NULL;
}

HANDLE wce_CreateFileForMappingA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
	HANDLE hTemplateFile)
{
	WCHAR wideStr[MAX_PATH];
	if(a2w_buffer(lpFileName, -1, wideStr, MAX_PATH))
	{
		return CreateFileForMappingW(wideStr, dwDesiredAccess, dwShareMode, lpSecurityAttributes, 
			dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}
	return NULL;
}


UINT wce_GetWindowsDirectoryA(LPSTR inBuffer, UINT inSize)
{
#ifdef DEBUG
    wce_printf("wce_GetWindowsDirectoryA called\n");
#endif

    UINT retval = 0;

    if(inSize < 9)
    {
        retval = 9;
    }
    else
    {
        strcpy(inBuffer, "\\WINDOWS");
        retval = 8;
    }

    return retval;
}

// WIN32W相关
DWORD wce_GetGlyphOutlineW(HDC inDC, WCHAR inChar, UINT inFormat, void* inGM, DWORD inBufferSize, LPVOID outBuffer, CONST void* inMAT2)
{
#ifdef DEBUG
	wce_printf("wce_GetGlyphOutlineW called\n");
#endif

	DWORD retval = GDI_ERROR;

	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);

	return retval;
}

DWORD SHGetValueW(HKEY hkey, LPCWSTR pszSubKey, LPCWSTR pszValue, DWORD *pdwType, void *pvData, DWORD *pcbData)
{
	HKEY hSubKey;
	if (ERROR_SUCCESS == RegOpenKeyExW(hkey, pszSubKey, 0, KEY_QUERY_VALUE, &hSubKey) && hSubKey)
	{
		DWORD r = RegQueryValueExW(hSubKey, pszValue, NULL, pdwType, (LPBYTE)pvData, pcbData);
		RegCloseKey(hSubKey);
		return r;
	}
	return ERROR_NOT_SUPPORTED;
}

UINT wce_GetWindowsDirectoryW(LPWSTR inBuffer, UINT inSize)
{
#ifdef DEBUG
    wce_printf("wce_GetWindowsDirectoryW called\n");
#endif

    UINT retval = 0;

    if(inSize < 9)
    {
        retval = 9;
    }
    else
    {
        wcscpy(inBuffer, L"\\WINDOWS");
        retval = 8;
    }

    return retval;
}

int wce_SHCreateDirectoryEx(HWND hwnd, LPCWSTR pszPath, SECURITY_ATTRIBUTES *psa)
{
	if (CreateDirectoryW(pszPath, psa))
		return ERROR_SUCCESS;
	else
		return ERROR_BAD_PATHNAME;
}

DWORD wce_GetFullPathName(LPCSTR lpFileName, 
                        DWORD nBufferLength, 
                        LPSTR lpBuffer, 
                        LPSTR *lpFilePart)
{
#ifdef DEBUG
    wce_printf("wce_GetFullPathName called\n");
#endif

    DWORD len = strlen(lpFileName);
    if (len > nBufferLength)
        return len;
    
    strncpy((char*)lpBuffer, lpFileName, len);
    ((char*)lpBuffer)[len] = '\0';
    
    if(lpFilePart)
    {
        char* sep = strrchr(lpBuffer, '\\');
        if (sep) {
            sep++; // pass the seperator
            *lpFilePart = sep;
        }
        else
            *lpFilePart = lpBuffer;
    }
    
#ifdef DEBUG
    wce_printf("wce_GetFullPathName called %s (%s)\n", lpBuffer, *lpFilePart);
#endif
    return len;	
}

// crgdbg.h相关
#if _WIN32_WCE < 0x500
BOOL IsDebuggerPresent(VOID)
{
	return FALSE;
}
#endif

int _CrtDbgReport(int, const char *, int, const char *, const char *, ...)
{
	return 0;
}

// 获取线程堆栈起始地址
typedef struct Process
{
	DWORD     _1[2];
	HANDLE    hProc; /**//* 08: handle for this process, needed only for SC_GetProcFromPtr */
} PROCESS, *PPROCESS;

typedef struct Thread
{
	DWORD     _1[3];
	PPROCESS  pProc;       /* 0C: pointer to current process */
	PPROCESS  pOwnerProc;  /* 10: pointer to owner process */
	DWORD     _2[2];
	DWORD     dwStackBase; /* 1C: stack base */
} THREAD, *PTHREAD; /**//* Thread */

typedef struct cinfo
{
	char      acName[4]; /**//* 00: object type ID string */
	uchar     disp; /**//* 04: type of dispatch */
	uchar     type; /**//* 05: api handle type */
	ushort    cMethods; /**//* 06: # of methods in dispatch table */
	const PFNVOID *ppfnMethods;/**//* 08: ptr to array of methods (in server address space) */
	const     DWORD *pdwSig; /**//* 0C: ptr to array of method signatures */
	PPROCESS  pServer; /**//* 10: ptr to server process */
} CINFO; /**//* cinfo */
typedef CINFO *PCINFO;

typedef struct _HDATA HDATA, *PHDATA;
struct _HDATA
{
	DWORD _1[2]; /**//* 00: links for active handle list */
	HANDLE hValue; /**//* 08: Current value of handle (nonce) */
	DWORD lock; /**//* 0C: access information */
	DWORD ref; /**//* 10: reference information */
	const CINFO *pci; /**//* 14: ptr to object class description structure */
	PVOID pvObj; /**//* 18: ptr to object */
	DWORD dwInfo; /**//* 1C: extra handle info */
}; /**//* 20: sizeof(HDATA) */

#ifdef x86
struct KDataStruct
{
	LPDWORD lpvTls; /**//* 0x000 Current thread local storage pointer */
	HANDLE ahSys[NUM_SYS_HANDLES]; /**//* 0x004 If this moves, change kapi.h */
	DWORD _1[4];
	ulong handleBase; /**//* 0x094 base address of handle table */
}; /**//* KDataStruct */
#endif

#ifdef ARM
struct KDataStruct
{
	LPDWORD lpvTls; /**//* 0x000 Current thread local storage pointer */
	HANDLE ahSys[NUM_SYS_HANDLES]; /**//* 0x004 If this moves, change kapi.h */
	DWORD _1[6];
	ulong handleBase; /**//* 0x09c handle table base address */
}; /**//* KDataStruct */
#endif

#define HandleToThread(h) ((THREAD *)GetObjectPtrByType((h), SH_CURTHREAD))
#define HANDLE_ADDRESS_MASK 0x1ffffffc

void h2p(HANDLE h, PHDATA& phdRet)
{
	if ((ulong)h < NUM_SYS_HANDLES+SYS_HANDLE_BASE && (ulong)h >= SYS_HANDLE_BASE)
		h = ((KDataStruct*)PUserKData)->ahSys[(uint)h-SYS_HANDLE_BASE];
	if (h)
	{
		phdRet = (PHDATA)(((ulong)h & HANDLE_ADDRESS_MASK) + ((KDataStruct*)PUserKData)->handleBase);
		if (phdRet->hValue != h)
			phdRet = 0;
	}
	else
		phdRet = 0;
}

PVOID GetObjectPtrByType(HANDLE h, int type)
{
	PHDATA phd;
	h2p(h, phd);
	return (phd && phd->pci && phd->pci->type==type) ? phd->pvObj : 0;
}

extern "C" LPVOID WINAPI MapPtrToProcess(LPVOID lpv, HANDLE hProc);
extern "C" BOOL WINAPI SetKMode(BOOL fMode);

LPVOID GetThreadStackBase(DWORD dwThreadId)
{
	LPVOID pStackBase = 0;
	BOOL fOldMode = SetKMode(TRUE);
#if 1
	{
		PTHREAD pTh = HandleToThread((HANDLE)dwThreadId);
		if (pTh)
			pStackBase = MapPtrToProcess((LPVOID)pTh->dwStackBase, pTh->pOwnerProc->hProc);
	}
#else // 另外一种方法
	{
		PTHREAD pth = (PTHREAD)((PHDATA)((dwThreadId & HANDLE_ADDRESS_MASK) + 0x80000000))->pvObj;
		if (pth)
			pStackBase = MapPtrToProcess((LPVOID)pth->dwStackBase, pth->pOwnerProc->hProc);
	}
#endif
	SetKMode(fOldMode);
	return pStackBase;
}

int *_errno (void)
{
	static int my_errno = 0;
	return &my_errno;
}

};
