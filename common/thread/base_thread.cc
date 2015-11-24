// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#include <unistd.h>
#include "base_thread.h"

ThreadBase::~ThreadBase()
{}

bool ThreadBase::Start()
{
    int ret = pthread_create(&_thread_id, NULL, ThreadEntry, this);
    _is_live = (ret == 0);

    return _is_live;
}

int ThreadBase::Join()
{
	return ::pthread_join(_thread_id, NULL);
}

void ThreadBase::Wait()
{
    pthread_join(_thread_id, NULL);
}

void ThreadBase::OnExit()
{
    _is_live = false;
}

// make sure execute before exit
void ThreadBase::Cleanup(void* param)
{
    ThreadBase* thread = static_cast<ThreadBase*>(param);
    thread->OnExit();
}

void *ThreadBase::ThreadEntry(void *param)
{
    ThreadBase* thread = static_cast<ThreadBase*>(param);
    thread->_is_live = true;
    pthread_cleanup_push(Cleanup, param);
    thread->Entry();
    thread->_is_live = false;
    pthread_cleanup_pop(true);
    return NULL;
}

void ThreadBase::Yield()
{
	// ::usleep(0);
	sched_yield();
}

bool ThreadBase::IsAlive() const
{
	return _is_live;
}

void ThreadBase::Entry()
{
}

void ThreadBase::Exit()
{
	pthread_exit(NULL);
}

int ThreadBase::GetId()
{
	return syscall(SYS_gettid);
}

void ThreadBase::Sleep(int64_t time_in_ms)
{
	if (time_in_ms >= 0)
	{
		timespec ts = { time_in_ms / 1000, (time_in_ms % 1000) * 1000000 };
		nanosleep(&ts, &ts);
	}
	else
	{
		pause();
	}
}
