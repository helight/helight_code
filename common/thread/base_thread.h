
// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//
#ifndef COMMON_THREAD_BASE_H
#define COMMON_THREAD_BASE_H

#include <pthread.h>
#include <syscall.h>
#include <stdint.h>

class ThreadBase
{
public:
    ThreadBase(){}
    virtual ~ThreadBase();

public:
    virtual bool Start();
    virtual void Entry();
    virtual int Join();
    virtual void OnExit();
    void Wait();
    bool IsAlive() const;

    void Exit();
    void Yield();
    void Sleep(int64_t time_in_ms);
    int GetId();
    bool IsMain();

    bool StopAndWaitForExit()
    {
        _bStopRequested = true;
        return true;
    }
    inline void SendStopRequest()
    {
        _bStopRequested = true;
    }

    inline bool IsStopRequested() const
    {
        return _bStopRequested;
    }
private:
    static void Cleanup(void* param);
    static void *ThreadEntry(void *param);

private:
    pthread_t _thread_id;
    bool _is_live;
    volatile bool _bStopRequested;
    volatile bool _IsAlive;
};

#endif
