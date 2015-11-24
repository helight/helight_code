// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-02-28
// Description:
//

/// time stamp in millisecond (1/1000 second)
int64_t GetTimeStampInMs();

/// time stamp in microsecond (1/1000000 second)
int64_t GetTimeStampInUs();

struct TimeUtils
{
    // Milliseconds always returns milliseconds(1/1000s) since Jan 1, 1970 GMT.
    static int64_t Milliseconds();
    //
    // Milliseconds always returns microseconds(1/1000000s) since Jan 1, 1970 GMT.
    static int64_t Microseconds();
    //
    // Returns the offset in hours between local time and GMT (or UTC) time.
    static int GetGMTOffset();

    static std::string GetCurTime();
    //
    static std::string GetCurMilliTime();
};
