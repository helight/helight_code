// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-09
// Description:
//

#include <assert.h>
#include <atomic>
#include <errno.h>
#include <limits.h>
#include <stdexcept>
#include <string.h>
#include <unistd.h>

#include "xtimer.h"

static volatile std::atomic<uint64_t> timer_id;

TimerTask::TimerTask() : _running_timer(0)
{
    timer_id = 0;
    StartThread();
}

TimerTask::~TimerTask()
{
    Stop();
}

uint64_t TimerTask::AddTimer(int64_t interval, bool is_period,
                                const Callback& callback)
{
    assert(interval >= 0);

    MutexLocker lock(_mutex);
    uint64_t id = NewTimerId();
    TimerEntry& timer = _timers[id];
    timer.interval = interval;
    timer.is_period = is_period;
    timer.callback = callback;
    timer.is_enabled = true;

    SetNextTimeout(id, interval);

    return id;
}

uint64_t TimerTask::NewTimerId()
{
    return ++timer_id;
}

void TimerTask::SetNextTimeout(uint64_t id, int64_t interval)
{
    assert(_mutex.IsLocked());

    int64_t prev_top_time;

    if (GetLatestTime(&prev_top_time)) {
        PushNextTimeout(id, interval);
        int64_t new_top_time = _timeouts.top().time;
        // reschedule timer only if the new time is the most early
        if (new_top_time < prev_top_time)
            _cond.Signal();
    } else {
        // timer manager is empty, worker thread is waiting infinitely
        PushNextTimeout(id, interval);
        _cond.Signal();
    }
}

void TimerTask::PushNextTimeout(uint64_t id, int64_t interval)
{
    Timeout timeout = { GetCurrentTime() + interval, id };
    _timeouts.push(timeout);
}

uint64_t TimerTask::AddOneshotTimer(int64_t interval,
                                       const Callback& callback)
{
    return AddTimer(interval, false, callback);
    // callback(interval);
}

uint64_t TimerTask::AddPeriodTimer(int64_t interval,
                                      const Callback& callback)
{
    return AddTimer(interval, true, callback);
}

bool TimerTask::RemoveTimerInLock(uint64_t id)
{
    TimerMap::iterator it = _timers.find(id);
    if (it != _timers.end()) {
        _timers.erase(it);
        return true;
    }
    return false;
}

bool TimerTask::RemoveTimer(uint64_t id)
{
    if (0 == id) {
        return false;
    }

    for ( ; ;)
    {
        if (id != _running_timer)
        {
            MutexLocker lock(_mutex);
            return RemoveTimerInLock(id);
        }
        usleep(100);
    }
}

bool TimerTask::GetLatestTime(int64_t* time) const
{
    assert(_mutex.IsLocked());

    if (_timeouts.empty())
        return false;

    *time = _timeouts.top().time;
    return true;
}

bool TimerTask::GetLatestTimeout(int64_t* time) const
{
    assert(_mutex.IsLocked());

    if (GetLatestTime(time)) {
        // convert to relative time
        int64_t now = GetCurrentTime();
        if (*time < now)
            *time = 0;
        else
            *time -= now;
        return true;
    }

    return false;
}

void TimerTask::Entry()
{
    do {
        {
            MutexLocker lock(_mutex);
            int64_t time;
            if (GetLatestTimeout(&time))
                _cond.Wait(_mutex, static_cast<int>(time));
            else
                _cond.Wait(_mutex);
        }
        Dispatch();
    } while(true);
}

void TimerTask::Dispatch()
{
    uint64_t id;
    // Make a copy. It's necessary because the entry in the map maybe changed
    // by it's callback.
    TimerEntry entry;

    while (DequeueTimeoutEntry(&id, &entry)) {
        // Run the callback in unlocked state
        if (entry.callback) {
            entry.callback(id);
        }

        MutexLocker locker(_mutex);
        _running_timer = 0;
        if (entry.is_period) {
            TimerMap::iterator it = _timers.find(id);
            if (it != _timers.end()) {
                if (it->second.is_enabled) {
                    SetNextTimeout(it->first, it->second.interval);
                }
            }
        }
    }
}


bool TimerTask::DequeueTimeoutEntry(uint64_t* id, TimerEntry* entry)
{
    for (;;) {
        MutexLocker locker(_mutex);

        if (_timeouts.empty() || _timeouts.top().time > GetCurrentTime())
            return false;

        Timeout timeout = _timeouts.top();
        _timeouts.pop();

        TimerMap::iterator it = _timers.find(timeout.timer_id);
        if (it == _timers.end())
            continue;

        // ignore outdated timeouts
        if (!it->second.is_enabled)
            continue;

        *id = timeout.timer_id;
        *entry = it->second;

        if (!entry->is_period) {
            _timers.erase(it);
        }

        _running_timer = *id;
        break;
    }

    return true;
}

void TimerTask::Stop()
{
    Join();
    MutexLocker locker(_mutex);
    while (!_timeouts.empty())
        _timeouts.pop();

    _timers.clear();
}

void TimerTask::StartThread()
{
    if (!Start()) { // start Entry
        std::string msg = strerror(errno);
        throw std::runtime_error("Can't start timer thread: " + msg);
    }
}

void TimerTask::GetStats(TimerTask::Stats* stats) const
{
    MutexLocker lock(_mutex);
    int64_t time = 0;
    stats->oneshot_timer_num = 0;
    stats->period_timer_num = 0;
    stats->estimate_runover_time = 0;
    TimerMap::const_iterator it;
    for (it = _timers.begin(); it != _timers.end(); ++it) {
        if (it->second.is_period) {
            stats->period_timer_num++;
            time = LLONG_MAX;
        } else {
            if (it->second.interval > time)
                time = it->second.interval;
            stats->oneshot_timer_num++;
        }
    }
    stats->estimate_runover_time = time;
}

