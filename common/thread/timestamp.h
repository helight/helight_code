// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#ifndef COMMON_TIMESTAMP_H
#define COMMON_TIMESTAMP_H

#include <stdint.h>

/// time stamp in millisecond (1/1000 second)
int64_t GetTimeStampInMs();

inline int64_t GetTimeStamp()
{
    return GetTimeStampInMs();
}

/// time stamp in microsecond (1/1000000 second)
int64_t GetTimeStampInUs();

inline int64_t GetCurrentTime()
{
    return GetTimeStampInMs();
}

void RelativeTimeInMillSecondsToAbsTimeInTimeSpec(
    int64_t relative_time_in_ms,
    timespec* ts
);
#endif // COMMON_TIMESTAMP_H
