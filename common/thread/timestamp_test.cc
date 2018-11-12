// All rights reserved.
// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#include <time.h>
#include <sys/time.h>
#include "gtest/gtest.h"
#include "timestamp.h"

extern int64_t SystemGetTimeStampInUs();

const int kLoopCount = 1000000;

TEST(Timestamp, SystemGetTimeStampInUs)
{
    for (int i = 0; i < kLoopCount; ++i)
        SystemGetTimeStampInUs();
}

TEST(Timestamp, GetTimeStampInUs)
{
    for (int i = 0; i < kLoopCount; ++i)
    {
        GetTimeStampInUs();
    }
}

TEST(Timestamp, GetTimeStampInMs)
{
    int64_t t0 = GetTimeStampInMs();
    for (int64_t i = 0; i < kLoopCount; ++i)
    {
        int64_t t = GetTimeStampInMs();
        int diff = t - t0;
        if (diff < 0)
        {
            if (diff < -5)
                printf("time backward %d ms\n", diff);
        }
        t0 = t;
    }
}

TEST(Timestamp, GetTimeStampInMsPrecision)
{
    const int kTestCount = kLoopCount;
    int64_t total_diff = 0;
    int64_t total_abs_diff = 0;
    for (int i = 0; i < kTestCount; ++i)
    {
        int64_t t0 = SystemGetTimeStampInUs();
        int64_t t1 = GetTimeStampInUs();
        int64_t t2 = SystemGetTimeStampInUs();
        if (t0 == t2 && t1 != t0)
        {
            int diff = t1 - t0;
            total_diff += diff;
            int abs_diff = abs(diff);
            total_abs_diff += abs_diff;
            if (abs_diff > 40)
            {
                printf("diff %d\n", diff);
                ASSERT_LE(abs_diff, 40);
            }
        }
    }
    printf("average total diff=%g\n", static_cast<double>(total_diff) / kTestCount);
    printf("average abs diff %g\n", static_cast<double>(total_abs_diff) / kTestCount);
}

TEST(Timestamp, gettimeofday)
{
    struct timeval tv;
    for (int i = 0; i < kLoopCount; ++i)
    {
        gettimeofday(&tv, NULL);
    }
}
