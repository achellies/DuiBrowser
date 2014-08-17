#if !defined(__time_conversions_h)
#define __time_conversions_h


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



#endif /* __time_conversions_h */
