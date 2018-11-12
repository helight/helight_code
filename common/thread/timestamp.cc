// All rights reserved.
// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#include <stddef.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/utsname.h>
#include "timestamp.h"


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

int64_t GetTimeStampInUs()
{
    return SystemGetTimeStampInUs();
}

int64_t GetTimeStampInMs()
{
    int64_t timestamp = SystemGetTimeStampInUs();
    return timestamp / 1000;
}

void RelativeTimeInMillSecondsToAbsTimeInTimeSpec(
    int64_t relative_time_in_ms,
    timespec* ts
)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t usec = tv.tv_usec + relative_time_in_ms * 1000LL;
    ts->tv_sec = tv.tv_sec + usec / 1000000;
    ts->tv_nsec = (usec % 1000000) * 1000;
}
