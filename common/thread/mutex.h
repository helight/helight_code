// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#ifndef COMMON_THREAD_MUTEX_H
#define COMMON_THREAD_MUTEX_H
#pragma once

#include <pthread.h>

class Uncopyable
{
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:  // emphasize the following members are private
    Uncopyable(const Uncopyable&);
    const Uncopyable& operator=(const Uncopyable&);
};

class Mutex
{
public:
    Mutex()
    {
        int type = PTHREAD_MUTEX_ERRORCHECK_NP;
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, type);
        pthread_mutex_init(&_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&_mutex);
    }

    void Lock()
    {
        pthread_mutex_lock(&_mutex);
    }

    void Unlock()
    {
        pthread_mutex_unlock(&_mutex);
    }
    bool IsLocked() const
    {
        // by inspect internal data
        return _mutex.__data.__lock > 0;
    }

private:
    Mutex(const Mutex& right);
    Mutex& operator = (const Mutex& right);
private:
    pthread_mutex_t _mutex;
    friend class ConditionVar;
};

template <typename LockType>
class ScopedLocker : private Uncopyable
{
public:
    explicit ScopedLocker(LockType& lock)
        : _lock(&lock)
    {
        _lock->Lock();
    }

    explicit ScopedLocker(LockType* lock)
        : _lock(lock)
    {
        _lock->Lock();
    }
    ~ScopedLocker()
    {
        _lock->Unlock();
    }
private:
    LockType* _lock;
};

typedef ScopedLocker<Mutex> MutexLocker;
#endif // COMMON_THREAD_MUTEX_H
