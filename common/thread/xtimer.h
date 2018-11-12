// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-09
// Description:
//

#ifndef COMMON_THREAD_XTIMER_H
#define COMMON_THREAD_XTIMER_H
#pragma once

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "base_thread.h"
#include "condition_var.h"
#include "mutex.h"
#include "timestamp.h"

typedef std::function<void (uint64_t)> Callback;

class TimerTask : private ThreadBase
{
public:
    TimerTask();
    ~TimerTask();

    struct Stats
    {
        size_t  oneshot_timer_num;
        size_t  period_timer_num;
        int64_t estimate_runover_time;
    };

public:
    /// @brief add a one-shot timer
    /// @param interval  timer interval, in milliseconds
    /// @param callback bind function
    /// @return unique timer id
    uint64_t AddOneshotTimer(int64_t interval, const Callback& callback);

    /// @brief add a periodly run timer
    /// @param interval  timer interval, in milliseconds
    /// @param callback bind function
    /// @return unique timer id
    uint64_t AddPeriodTimer(int64_t interval, const Callback& callback);
    /// @brief remove a timer, if timer is running, wait for complete
    /// @id timer id
    /// @return ok
    bool RemoveTimer(uint64_t id);

    /// @brief stats info of timer manager
    void GetStats(Stats* stats) const;

private:
    struct TimerEntry
    {
        TimerEntry() : interval(0), is_enabled(false), is_period(false)
        {}
        // Callback callback; ///< callback function
        Callback callback; ///< callback function
        int64_t interval;     ///< timer interval
        bool    is_enabled;   ///< whether timer is enabled
        bool    is_period;    ///< whether timer run periodly
    };

    struct Timeout
    {
        int64_t  time;        ///< time when the timer triggers
        uint64_t timer_id;    ///< timer id
        bool operator > (const Timeout& rhs) const
        {
            return time > rhs.time;
        }
    };

    typedef std::map<uint64_t, TimerEntry> TimerMap;

private:
    /// @brief timer thread start
    void StartThread();

    /// @brief timer thread entrance.
    virtual void Entry();
    bool GetLatestTimeout(int64_t* time) const;
    bool GetLatestTime(int64_t* time) const;
    void Dispatch();
    bool DequeueTimeoutEntry(uint64_t* id, TimerEntry* entry);

    uint64_t AddTimer(int64_t interval, bool is_period, const Callback& callback);
    bool RemoveTimerInLock(uint64_t id);

    uint64_t NewTimerId();
    void SetNextTimeout(uint64_t id, int64_t interva);
    void PushNextTimeout(uint64_t id, int64_t interval);

    void Stop();

private:
    mutable Mutex _mutex;
    ConditionVar _cond;
    TimerMap _timers; ///< times container
    /// timeouts queue, latest timeout first.
    std::priority_queue<Timeout, std::vector<Timeout>, std::greater<Timeout>> _timeouts;
    volatile uint64_t _running_timer;
};
#endif // COMMON_THREAD_XTIMER_H
