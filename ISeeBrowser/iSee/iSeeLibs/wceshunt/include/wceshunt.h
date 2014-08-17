#ifndef WCE_SHUNT_H
#define WCE_SHUNT_H

// This is to silence the #pragma deprecated warnings
#pragma warning(disable: 4068)

#include "wcedefs.h"

//////////////////////////////////////////////////////////
// Function Mapping
//////////////////////////////////////////////////////////

// From direct.cpp
#ifdef _mkdir
#undef _mkdir
#endif
#define _mkdir		mkdir

#ifdef _rmdir
#undef _rmdir
#endif
#define _rmdir		rmdir

#ifdef _chdir
#undef _chdir
#endif
#define _chdir		chdir

// From dirent.h
#ifdef opendir
#undef opendir
#endif
#define opendir     wce_opendir
#ifdef closedir
#undef closedir
#endif
#define closedir    wce_closedir
#ifdef readdir
#undef readdir
#endif
#define readdir     wce_readdir

// From io.cpp
#ifdef chmod
#undef chmod
#endif
#define chmod		wce_chmod

#ifdef _isatty
#undef _isatty
#endif
#define _isatty		wce_isatty

#ifdef isatty
#undef isatty
#endif
#define isatty		wce_isatty

#ifdef fileno
#undef fileno
#endif
#define fileno		wce_fileno

#ifdef _fileno
#undef _fileno
#endif
#define _fileno		wce_fileno

// From process.cpp
#ifdef abort
#undef abort
#endif
#define abort		wce_abort

#ifdef getenv
#undef getenv
#endif
#define getenv		wce_getenv

#ifdef setenv
#undef setenv
#endif
#define setenv		wce_setenv

#ifdef getpid
#undef getpid
#endif
#define getpid		wce_getpid

// From signal.cpp
#ifdef raise
#undef raise
#endif
#define raise		wce_raise

#ifdef signal
#undef signal
#endif
#define signal		wce_signal

// From stat.cpp
#ifdef stat
#undef stat
#endif
#define stat		wce_stat

#ifdef wstat
#undef wstat
#endif
#define wstat		wce_wstat
#define _wstat      wstat

// From stdio.cpp
#ifdef access
#undef access
#endif
#define access		wce_access

#ifdef _access
#undef _access
#endif
#define _access		wce_access

#ifdef rewind
#undef rewind
#endif
#define rewind		wce_rewind

#ifdef _fdopen
#undef _fdopen
#endif
#define	_fdopen		wce_fdopen

#ifdef fdopen
#undef fdopen
#endif
#define	fdopen		wce_fdopen

#ifdef perror
#undef perror
#endif
#define	perror		wce_perror

#ifdef _getcwd
#undef _getcwd
#endif
#define _getcwd     wce_getcwd

#ifdef getcwd
#undef getcwd
#endif
#define getcwd      wce_getcwd

#ifndef NO_PRINTF
#ifdef printf
#undef printf
#endif
#define printf wce_printf
#endif

#ifdef _open
#undef _open
#endif
#define _open open

#ifdef _close
#undef _close
#endif
#define _close close

#ifdef _read
#undef _read
#endif
#define _read read

#ifdef _write
#undef _write
#endif
#define _write write

#ifdef unlink
#undef unlink
#endif
#define unlink wce_unlink

#ifdef lseek
#undef lseek
#endif
#define lseek wce_lseek

#ifdef _lseek
#undef _lseek
#endif
#define _lseek lseek

#ifdef setvbuf
#undef setvbuf
#endif
#define setvbuf wce_setvbuf

#ifdef fstat
#undef fstat
#endif
#define fstat wce_fstat

#ifdef _fstat
#undef _fstat
#endif
#define _fstat fstat

// From stdlib.cpp
#ifdef _fullpath
#undef _fullpath
#endif
#define _fullpath	wce_fullpath

#ifdef _splitpath
#undef _splitpath
#endif
#define _splitpath	wce_splitpath

#ifdef _makepath
#undef _makepath
#endif
#define _makepath	wce_makepath

#ifdef strdup
#undef strdup
#endif
#define strdup      _strdup

#ifdef stricmp
#undef stricmp
#endif
#define stricmp     _stricmp

#ifdef strcmpi
#undef strcmpi
#endif
#define strcmpi     _stricmp

#ifdef strnicmp
#undef strnicmp
#endif
#define strnicmp    _strnicmp

#ifdef rand_s
#undef rand_s
#endif
#define rand_s      wce_rand_s

// From string.cpp
#ifdef strerror
#undef strerror
#endif
#define strerror	wce_strerror

#ifndef NO_PRINTF

#ifdef snprintf
#undef snprintf
#endif
#define snprintf    xsnprintf

#ifdef vsnprintf
#undef vsnprintf
#endif
#define vsnprintf   xvsnprintf

#ifdef _vscprintf
#undef _vscprintf
#endif
#define _vscprintf  xvscprintf

#endif

#define errno   (*_errno())

//////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// From Unistd.cpp  
WCE_SHUNT_API int chown(const char *__path, uid_t __owner, gid_t __group);
WCE_SHUNT_API int fchmod(int __fd, mode_t __mode);
WCE_SHUNT_API int fchown(int __fd, uid_t __owner, gid_t __group );
WCE_SHUNT_API int lstat( const char *__path, struct stat *__buf);

/* On win32 we do not have fs-links, so simply 0 (success) is returned
 when __path is accessible. It is then just copied to __buf.
*/
WCE_SHUNT_API int readlink(const char *__path, char *__buf, int __buflen);
/* just copies __name1 to __name2 */
WCE_SHUNT_API int symlink(const char *__name1, const char *__name2);
/* just copies __name1 to __name2 */
WCE_SHUNT_API int link(const char *__name1, const char *__name2);

WCE_SHUNT_API int wce_gethostname(char *__name, size_t __len);
WCE_SHUNT_API unsigned alarm(unsigned __secs );
WCE_SHUNT_API char* getlogin();
WCE_SHUNT_API int fsync (int fd);
WCE_SHUNT_API void usleep(unsigned int usec);
WCE_SHUNT_API void sleep(unsigned int sec);
WCE_SHUNT_API long int random();
WCE_SHUNT_API int setreuid(uid_t ruid, uid_t euid);
WCE_SHUNT_API int fcntl(int fd, int cmd,...);
WCE_SHUNT_API int mkstemps (char* _template, int suffix_len);
WCE_SHUNT_API char* mkdtemp (char* _template);
WCE_SHUNT_API FILE *tmpfile();
  
// From direct.cpp
WCE_SHUNT_API int mkdir(const char* inDirname);
WCE_SHUNT_API int rmdir(const char* inDirname);
WCE_SHUNT_API int chdir(const char *inDirname);

// From dirent.cpp
WCE_SHUNT_API DIR *wce_opendir( const char * path );
WCE_SHUNT_API struct dirent *wce_readdir( DIR *d );
WCE_SHUNT_API int wce_closedir( DIR *d );

// From io.cpp
WCE_SHUNT_API int wce_chmod(const char* inFilename, int inMode);
WCE_SHUNT_API int wce_isatty(int inHandle);
WCE_SHUNT_API int wce_fileno(FILE* inHandle);

// From process.cpp
WCE_SHUNT_API void wce_abort(void);
WCE_SHUNT_API char* wce_getenv(const char* inName);
WCE_SHUNT_API int wce_setenv(const char *inName, const char *inValue);
WCE_SHUNT_API int wce_getpid(void);

// From signal.cpp
WCE_SHUNT_API int wce_raise(int inSignal);
WCE_SHUNT_API _sigsig wce_signal(int inSignal, _sigsig inFunc);

// From stat.cpp
WCE_SHUNT_API int wce_stat(const char *, struct _stat *);
WCE_SHUNT_API int wce_wstat(const WCHAR *, struct _stat *);

// From stdio.cpp
WCE_SHUNT_API int wce_access(const char *path, int mode);
WCE_SHUNT_API void wce_rewind(FILE* inStream);
WCE_SHUNT_API FILE* wce_fdopen(int inFD, const char* inMode);
WCE_SHUNT_API void wce_perror(const char* inString);
WCE_SHUNT_API int remove(const char* inPath);
WCE_SHUNT_API int rename(const char* oldname, const char *newname);

WCE_SHUNT_API char* wce_getcwd(char* buff, size_t size);

WCE_SHUNT_API int wce_printf(const char *, ...);

// From xprintf.cpp
WCE_SHUNT_API int xvsnprintf(char *, size_t, const char *, va_list);
WCE_SHUNT_API int xsnprintf(char *, size_t, const char *, ...);
WCE_SHUNT_API int xvscprintf(const char *, va_list);

WCE_SHUNT_API int open(const char *pathname, int mode, ...);
WCE_SHUNT_API int close(int fp);
WCE_SHUNT_API size_t read(int fp, void* buffer, size_t count);
WCE_SHUNT_API size_t write(int fp, const void* buffer, size_t count);
WCE_SHUNT_API int wce_unlink(const char *pathname);
WCE_SHUNT_API int wce_lseek(int fildes, int offset, int whence);
WCE_SHUNT_API int wce_setvbuf(FILE *stream, char *buffer, int mode, size_t size);

WCE_SHUNT_API int wce_fstat(int fd, struct stat *);

// From stdlib.cpp
WCE_SHUNT_API void wce_splitpath(const char* inPath, char* outDrive, char* outDir, char* outFname, char* outExt);
WCE_SHUNT_API void wce_makepath(char* outPath, const char* inDrive, const char* inDir, const char* inFname, const char* inExt);
WCE_SHUNT_API char* wce_fullpath(char *, const char *, size_t);
WCE_SHUNT_API void *bsearch(const void *key, const void *base, size_t high,
                            size_t size, int (*compar)(const void *, const void *));
WCE_SHUNT_API errno_t rand_s(unsigned int *);

// From string.cpp
WCE_SHUNT_API char* wce_strerror(int);

// From time.cpp
WCE_SHUNT_API struct tm* localtime_r(const time_t* inTimeT,struct tm* outRetval);
WCE_SHUNT_API struct tm* gmtime_r(const time_t* inTimeT, struct tm* outRetval);
WCE_SHUNT_API errno_t localtime_s(struct tm * _Tm, const time_t *_Time);
WCE_SHUNT_API void _ftime(struct wce__timeb * _Time);

// For errno
WCE_SHUNT_API int *_errno (void);

#ifdef __cplusplus
};
#endif

#ifdef USE_WCEEXTS_H
#include "wceexts.h"
#endif

#ifdef NO_DEFINES
#include "nodefines.h"
#endif

#ifdef NO_STRICT
#undef STRICT
#endif

#endif //WCE_SHUNT_H
