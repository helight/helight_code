// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#ifndef COMMON_THREAD_CONDITIONVAR_H
#define COMMON_THREAD_CONDITIONVAR_H

#include <assert.h>
#include "timestamp.h"
#include "mutex.h"

class ConditionVar
{
public:
    ConditionVar()
    {
        pthread_condattr_t cond_attr;
        pthread_condattr_init(&cond_attr);
        pthread_cond_init(&_cond, &cond_attr);
        pthread_condattr_destroy(&cond_attr);
    }

    ~ConditionVar()
    {
        pthread_cond_destroy(&_cond);
    }

    void Signal()
    {
        CheckValid();
        pthread_cond_signal(&_cond);
    }

    void Broadcast()
    {
        CheckValid();
        pthread_cond_broadcast(&_cond);
    }

    void Wait(Mutex* mutex)
    {
        CheckValid();
        pthread_cond_wait(&_cond, &mutex->_mutex);
    }

    void Wait(Mutex& mutex)
    {
        Wait(&mutex);
    }

    bool Wait(Mutex* mutex, int timeout_in_ms)
    {
        // -1 转为无限等
        if (timeout_in_ms < 0)
        {
            Wait(mutex);
            return true;
        }

        timespec ts;
        RelativeTimeInMillSecondsToAbsTimeInTimeSpec(timeout_in_ms, &ts);
        return (0 == pthread_cond_timedwait(&_cond, &mutex->_mutex, &ts));
    }

    bool Wait(Mutex& mutex, int timeout_in_ms)
    {
        return Wait(&mutex, timeout_in_ms);
    }

private:
    void CheckValid() const
    {
        // __total_seq will be set to -1 by pthread_cond_destroy
        assert(_cond.__data.__total_seq != -1ULL && "this cond has been destructed");
    }

private:
    pthread_cond_t _cond;
};

#endif
