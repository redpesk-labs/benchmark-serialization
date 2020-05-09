#pragma once

#define NUMBER_PORT				1
#define PARSING_BUFFER_SIZE		14

#define DATA_TESTED				10000

#ifdef BENCH_JSON
#include "parse-json.h"
#endif

#ifdef BENCH_CBOR
#include "parse-cbor.h"
#endif

#if BENCH_XDR
#include "parse-xdr.h"
#endif

// Portability fix: on windows there is no posix clock
// This code provides a precision clock using the performance counters using
// the same interface as the posix clock.
#ifdef _MSC_VER

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <sdkddkver.h>
#pragma warning( push )
#pragma warning( disable: 4005)
#define WINVER										_WIN32_WINNT_WIN10
#define _WIN32_WINNT								_WIN32_WINNT_WIN10
#define NTDDI_VERSION								NTDDI_WIN10_RS5
#pragma warning( pop )

#include <Windows.h>
#define CLOCK_REALTIME								0
#define CLOCK_MONOTONIC								0
#define CLOCK_PROCESS_CPUTIME_ID					0
#define CLOCK_THREAD_CPUTIME_ID						0
#define exp7           10000000i64					//1E+7
#define exp9         1000000000i64					//1E+9
#define w2ux 116444736000000000i64					//1.jan1601 to 1.jan1970

typedef int clockid_t;
struct timespec { uint64_t tv_sec; uint64_t tv_nsec; };		//header part

inline void unix_time(struct timespec* spec)
{
	__int64 wintime;
	GetSystemTimeAsFileTime((FILETIME*)&wintime);
	wintime -= w2ux;
	spec->tv_sec = wintime / exp7;
	spec->tv_nsec = wintime % exp7 * 100;
}

inline int clock_gettime(clockid_t clk_id, struct timespec* spec)
{
	static struct timespec startspec;
	static double ticks2nano;
	static __int64 startticks, tps = 0;
	__int64 tmp, curticks;

	QueryPerformanceFrequency((LARGE_INTEGER*)&tmp);

	if (tps != tmp)
	{
		tps = tmp;
		QueryPerformanceCounter((LARGE_INTEGER*)&startticks);
		unix_time(&startspec); ticks2nano = (double)exp9 / tps;
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&curticks);
	curticks -= startticks;
	spec->tv_sec = startspec.tv_sec + (curticks / tps);
	spec->tv_nsec = startspec.tv_nsec + (double)(curticks % tps) * ticks2nano;
	if (!(spec->tv_nsec < exp9))
	{
		spec->tv_sec++;
		spec->tv_nsec -= exp9;
	}
	return 0;
}

#else
#include <time.h>
#endif
