#ifndef WCE_DEFS_H
#define WCE_DEFS_H

#include <stdlib.h>
#include <time.h>
#include <yvals.h>
#include <sys\types.h>

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#ifdef WCESHUNT_STATICLIB
#define WCE_SHUNT_API
#else
#ifdef WCE_SHUNT_EXPORTS
#define WCE_SHUNT_API __declspec(dllexport)
#else
#define WCE_SHUNT_API __declspec(dllimport)
#endif
#endif

// ×Ö½Ú¶ÔÆä
#define MY_ALIGN(A,L)	(((A) + (L) - 1) & ~((L) - 1))
#define ALIGN_INT(A)	MY_ALIGN((A), sizeof(int))

//////////////////////////////////////////////////////////
// Various Definations
//////////////////////////////////////////////////////////

// From signal.h
#define SIGABRT         0
#define SIGSEGV         1
#define _SIGCOUNT       2 /* LAST ONE, SIZES BUFFER */

typedef void (*_sigsig)(int inSignal);

// From stdio.h

#define BUFSIZ         512
#define O_RDONLY       0x0000  // open for reading only
#define O_WRONLY       0x0001  // open for writing only
#define O_RDWR         0x0002  // open for reading and writing
#define O_APPEND       0x0008  // writes done at eof

#define O_TEXT         0x4000  // file mode is text (translated)
#define O_BINARY       0x8000  // file mode is binary (untranslated)

#define O_CREAT        0x0100  // create and open file
#define O_TRUNC        0x0200  // open and truncate
#define O_EXCL         0x0400  // open only if file doesn't already exist

#define _O_TEMPORARY    0x0040  // temporary file bit

#define _O_CREAT        O_CREAT
#define _O_TRUNC        O_TRUNC
#define _O_RDONLY       O_RDONLY
#define _O_WRONLY       O_WRONLY
#define _O_RDWR         O_RDWR
#define _O_APPEND       O_APPEND

// From sys/types.h
typedef int ptrdiff_t;
typedef long _off_t;
typedef long off_t;

typedef unsigned int pid_t;
typedef unsigned int __uid_t;
typedef __uid_t uid_t;
typedef unsigned int __gid_t;
typedef __gid_t gid_t;
typedef char* caddr_t;
typedef int mode_t;
typedef int ssize_t;
typedef int dev_t;
typedef int ino_t;
typedef unsigned char UInt8;

#define	F_OK	0
#define	R_OK	4
#define	W_OK	2
#define	X_OK	1 

/* + from <fcntl.h> */
#define O_NDELAY	_FNDELAY

#define	_FNONBLOCK	0x4000	/* non blocking I/O (POSIX style) */ 
#define	O_NONBLOCK	_FNONBLOCK 

#define	_FOPEN		(-1)	/* from sys/file.h, kernel use only */
#define	_FREAD		0x0001	/* read enabled */
#define	_FWRITE		0x0002	/* write enabled */
#define	_FAPPEND	0x0008	/* append (writes guaranteed at the end) */
#define	_FMARK		0x0010	/* internal; mark during gc() */
#define	_FDEFER		0x0020	/* internal; defer for next gc pass */
#define	_FASYNC		0x0040	/* signal pgrp when data ready */
#define	_FSHLOCK	0x0080	/* BSD flock() shared lock present */
#define	_FEXLOCK	0x0100	/* BSD flock() exclusive lock present */
#define	_FCREAT		0x0200	/* open with file create */
#define	_FTRUNC		0x0400	/* open with truncation */
#define	_FEXCL		0x0800	/* error on open if file exists */
#define	_FNBIO		0x1000	/* non blocking I/O (sys5 style) */
#define	_FSYNC		0x2000	/* do all writes synchronously */
#define	_FNONBLOCK	0x4000	/* non blocking I/O (POSIX style) */
#define	_FNDELAY	_FNONBLOCK	/* non blocking I/O (4.2 style) */
#define	_FNOCTTY	0x8000	/* don't assign a ctty on this open */

#define	O_ACCMODE	(O_RDONLY|O_WRONLY|O_RDWR)

/* XXX close on exec request; must match UF_EXCLOSE in user.h */
#define	FD_CLOEXEC	1	/* posix */

/* fcntl(2) requests */
#define	F_DUPFD		0	/* Duplicate fildes */
#define	F_GETFD		1	/* Get fildes flags (close on exec) */
#define	F_SETFD		2	/* Set fildes flags (close on exec) */
#define	F_GETFL		3	/* Get file flags */
#define	F_SETFL		4	/* Set file flags */

#ifndef	_POSIX_SOURCE
# define	F_GETOWN 	5	/* Get owner - for ASYNC */
# define	F_SETOWN 	6	/* Set owner - for ASYNC */
#endif	/* !_POSIX_SOURCE */

#define	F_GETLK  	7	/* Get record-locking information */
#define	F_SETLK  	8	/* Set or Clear a record-lock (Non-Blocking) */
#define	F_SETLKW 	9	/* Set or Clear a record-lock (Blocking) */

#ifndef	_POSIX_SOURCE
# define	F_RGETLK 	10	/* Test a remote lock to see if it is blocked */
# define	F_RSETLK 	11	/* Set or unlock a remote lock */
# define	F_CNVT 		12	/* Convert a fhandle to an open fd */
# define	F_RSETLKW 	13	/* Set or Clear remote record-lock(Blocking) */
#endif	/* !_POSIX_SOURCE */

/* fcntl(2) flags (l_type field of flock structure) */
#define	F_RDLCK		1	/* read lock */
#define	F_WRLCK		2	/* write lock */
#define	F_UNLCK		3	/* remove lock(s) */

#ifndef	_POSIX_SOURCE
# define	F_UNLKSYS	4	/* remove remote locks for a given system */
#endif	/* !_POSIX_SOURCE */

// From sys/stat.h
#if !defined(_STAT_DEFINED)
#define _STAT_DEFINED

#define	_IFMT		0170000	/* type of file */
#define	_IFDIR		0040000	/* directory */
#define	_IFCHR		0020000	/* character special */
#define	_IFBLK		0060000	/* block special */
#define	_IFREG		0100000	/* regular */
#define	_IFLNK		0120000	/* symbolic link */
#define	_IFSOCK		0140000	/* socket */
#define	_IFIFO		0010000	/* fifo */

#define S_BLKSIZE  1024 // size of a block 

#define	S_ISUID		0004000	// set user id on execution 
#define	S_ISGID		0002000	// set group id on execution 
#define	S_ISVTX		0001000	// save swapped text even after use 

#define	S_IFBLK		_IFBLK
#define	S_IFLNK		_IFLNK
#define	S_IFSOCK	_IFSOCK
#define	S_IFIFO		_IFIFO

#define	S_IRWXU 	(S_IRUSR | S_IWUSR | S_IXUSR)
#define	S_IRUSR		0000400	/* read permission, owner */
#define	S_IWUSR		0000200	/* write permission, owner */
#define	S_IXUSR		0000100/* execute/search permission, owner */
#define	S_IRWXG		(S_IRGRP | S_IWGRP | S_IXGRP)
#define	S_IRGRP		0000040	/* read permission, group */
#define	S_IWGRP		0000020	/* write permission, grougroup */
#define	S_IXGRP		0000010/* execute/search permission, group */
#define	S_IRWXO		(S_IROTH | S_IWOTH | S_IXOTH)
#define	S_IROTH		0000004	/* read permission, other */
#define	S_IWOTH		0000002	/* write permission, other */
#define	S_IXOTH		0000001/* execute/search permission, other */

#define	S_ISBLK(m)	(((m)&_IFMT) == _IFBLK)
#define	S_ISCHR(m)	(((m)&_IFMT) == _IFCHR)
#define	S_ISDIR(m)	(((m)&_IFMT) == _IFDIR)
#define	S_ISFIFO(m)	(((m)&_IFMT) == _IFIFO)
#define	S_ISREG(m)	(((m)&_IFMT) == _IFREG)
#define	S_ISLNK(m)	(((m)&_IFMT) == _IFLNK)
#define	S_ISSOCK(m)	(((m)&_IFMT) == _IFSOCK)

#define _S_IFMT     0140000 /* stat, file type mask */
#define _S_IFDIR    0040000 /* stat, is a directory */
#define _S_IFREG    0100000 /* stat, is a normal file */
#define _S_IREAD    0000400 /* stat, can read */
#define _S_IWRITE   0000200 /* stat, can write */
#define	_S_IEXEC	0000100

#define S_IFMT      _S_IFMT
#define S_IFDIR     _S_IFDIR
#define S_IFREG     _S_IFREG
#define S_IREAD     _S_IREAD
#define S_IWRITE    _S_IWRITE
#define	S_IEXEC     _S_IEXEC

struct wce_stat
{
    unsigned short st_mode;
    _off_t st_size;
    time_t st_ctime;
    time_t st_atime;
    time_t st_mtime;
	int st_gid;
	int st_uid;
	dev_t st_dev;
	ino_t st_ino;
};

#define _stat wce_stat
#define stat wce_stat
#endif /* _STAT_DEFINED */

#ifndef _TIMEB_DEFINED
#define _TIMEB_DEFINED
struct wce__timeb {
        time_t time;
        unsigned short millitm;
        short timezone;
        short dstflag;
        };
#define _timeb      wce__timeb
#define timeb       wce__timeb
#endif

#ifndef _DIRENT_DEFINED
#define _DIRENT_DEFINED
struct dirent
{
    char *d_name; // valid as long as the DIR is valid
};
typedef struct _DIR DIR;
#endif

#endif // WCE_DEFS_H
