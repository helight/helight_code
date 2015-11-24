// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-02-28
// Description:
//
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include <string>

#include "time_utils.h"

/// get system time, in microseconds
int64_t SystemGetTimeStampInUs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t result = tv.tv_sec;
    result *= 1000000;
    result += tv.tv_usec;
    return result;
}
/// read cpu timestamp count
static inline int64_t rdtsc()
{
    int64_t tsc;
#if defined __i386__
    __asm__ __volatile__("rdtsc" : "=A" (tsc));
#elif defined __x86_64
    unsigned int a, d;
    __asm__ __volatile__("rdtsc" : "=a" (a), "=d" (d));
    tsc = static_cast<int64_t>(a) | (static_cast<int64_t>(d) << 32);
#else
#error unsupported platform
#endif
    return tsc;
}

/// @brief get both timestamp and tsc
/// @retval true no context switch, timestamp and tsc same timeslice
/// @retval false context switched, only timestamp is trustable
static bool get_timestamp_and_tsc(int64_t* timestamp, int64_t* tsc)
{
    int64_t tsc1 = rdtsc();
    *timestamp = SystemGetTimeStampInUs();
    int64_t tsc2 = rdtsc();

    // the difference is typical several hundreds if no context switching
    if (tsc2 > tsc1 && tsc2 - tsc1 < 50000)
    {
        // no context switched between 2 rdtsc() call
        // rounded avarage value
        *tsc = tsc1 + (tsc2 - tsc1 + 1) / 2;
        return true;
    }
    else
    {
        // context switched between 2 rdtsc() callings,
        // can not decide which tsc is useable
        return false;
    }
}

// if kernel support vsyscall64, time call will be 10 times faster.
// check it.
int get_vsyscall64_value()
{
    int result = -1;
    int fd = open("/proc/sys/kernel/vsyscall64", O_RDONLY, 0);
    if (fd >= 0)
    {
        char buf[2];
        int num_read = read(fd, buf, 2);
        if (num_read >= 1)
        {
            switch (buf[0])
            {
            case '0':
            case '1':
            case '2':
                result = buf[0] - '0';
            }
        }
        close(fd);
    }
    return result;
}

static bool HasFastGetTimeOfDay()
{
#ifdef __x86_64__
    // For x86_64 application, as we know, gettimeofday become fast since at
    // least 2.6.32. It cost 47ms to call 1000000 times on this machine
    // (Intel(R) Xeon(R) CPU L5630 @ 2.13GHz).
    utsname u;
    if (uname(&u) < 0)
        return false;
    int major, minor, patch;
    if (sscanf(u.release, "%d.%d.%d", &major, &minor, &patch) != 3)
        return false;
    int version = major * 10000 + minor * 100 + patch;
    if (version < 20632) // 2.6.32
        return false;
    if (strcmp(u.machine, "x86_64") != 0)
        return false;
    return true;
#endif

    // But 32 bit is still not fast enough, It cost about 120ms to call
    // 1000000 times on the same above machine.
    return false;
}


int64_t GetTimeStampInUs()
{
    // initialized.
    return SystemGetTimeStampInUs();
}

int64_t GetTimeStampInMs()
{
    int64_t timestamp = GetTimeStampInUs();
    return timestamp / 1000;
}

int64_t TimeUtils::Milliseconds()
{
    return GetTimeStampInMs();
}

int64_t TimeUtils::Microseconds()
{
    return GetTimeStampInUs();
}

int TimeUtils::GetGMTOffset()
{
    struct timeval tv;
    struct timezone tz;

    int err = ::gettimeofday(&tv, &tz);
    if (err != 0)
        return 0;

    return ((tz.tz_minuteswest / 60) * -1); // return hours before or after GMT
}

std::string TimeUtils::GetCurTime()
{
    std::string retStr;
    char s[50];
    struct tm curr;

    time_t iCurTime;

    time(&iCurTime);

    curr = *localtime(&iCurTime);

    if (curr.tm_year > 50)
    {
        sprintf(s, "%04d%02d%02d%02d%02d%02d",
                curr.tm_year+1900, curr.tm_mon+1, curr.tm_mday,
                curr.tm_hour, curr.tm_min, curr.tm_sec);
    }
    else
    {
        sprintf(s, "%04d%02d%02d%02d%02d%02d",
                curr.tm_year+2000, curr.tm_mon+1, curr.tm_mday,
                curr.tm_hour, curr.tm_min, curr.tm_sec);
    }
    retStr = s;
    return retStr;
}

std::string TimeUtils::GetCurMilliTime()
{
    std::string retStr;
    char s[50];
    struct tm curr;

    int64_t nowMilliSecs = TimeUtils::Milliseconds();
    time_t iCurTime = nowMilliSecs/1000;

    curr = *localtime(&iCurTime);

    if (curr.tm_year > 50)
    {
        sprintf(s, "%04d%02d%02d%02d%02d%02d%03d",
                curr.tm_year+1900, curr.tm_mon+1, curr.tm_mday,
                curr.tm_hour, curr.tm_min, curr.tm_sec, (int)(nowMilliSecs%1000));
    }
    else
    {
        sprintf(s, "%04d%02d%02d%02d%02d%02d%03d",
                curr.tm_year+2000, curr.tm_mon+1, curr.tm_mday,
                curr.tm_hour, curr.tm_min, curr.tm_sec, (int)(nowMilliSecs%1000));
    }
    retStr = s;
    return retStr;
}
