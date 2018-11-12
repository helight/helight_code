// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#ifndef COMMON_THREAD_POOL_H_
#define COMMON_THREAD_POOL_H_

#include <stddef.h>

#include <string>

#include "condition_var.h"
#include "mutex.h"
#include "thread.h"


typedef void (*StartRoutine)(void* context, unsigned long long param);

/// Thread pool class, similarly to the one of .NET
class ThreadPool
{
public:
    struct Stats
    {
        size_t NumThreads;
        size_t NumBusyThreads;
        size_t NumPendingTasks;
    };

    /// Conctruct a named thread_pool object
    ThreadPool(
        const std::string& name,   ///< human readable name, for debug purpose
        int min_concurrencys = 0,  ///< -1 means cpu number
        int max_concurrencys = -1, ///< -1 means cpu number
        int idle_timeout = 60000,  ///< max idle time, in milliseconds
        size_t stack_size = 0      ///< the stack size, in bytes, 0 means default
    );

    /// Conctruct a anonymous thread_pool object
    ThreadPool(
        int min_concurrencys = 0,  ///< -1 means cpu number
        int max_concurrencys = -1, ///< -1 means cpu number
        int idle_timeout = 60000,  ///< max idle time, in milliseconds
        size_t stack_size = 0      ///< the stack size, in bytes, 0 means default
    );
    ~ThreadPool();

    /// Set attributes
    void SetMinThreads(int size);
    void SetMaxThreads(int size);
    void SetIdleTime(int time);

    int GetMinThreads() const { return m_MinThreads; }
    int GetMaxThreads() const { return m_MaxThreads; }
    int GetIdleTime() const { return m_IdleTime; }

    /// This function is deprecated, please using Closure/Function as the type
    /// of the task
    void AddTask(
        StartRoutine routine,
        void* context = NULL,
        unsigned long long param = 0
    );

    /// Add a task to thread pool, will be run in the future
    /// void AddTask(Closure<void>* callback);
    /// void AddTask(const Function<void ()>& function);

    /// Add a task to thread pool, will be run in the future, ASAP
    /// void AddPriorityTask(Closure<void>* callback);
    /// void AddPriorityTask(const Function<void ()>& function);

    /// Terminate the thread pool.
    /// @param wait if it's true, wait for all pending task to complete,
    ///             otherwise all pending tasks will be discarded.
    void Terminate(bool wait = true);

    void GetStats(Stats* stat) const;

    /// Wait for all pending tasks to be done, but not terminate.
    void WaitForIdle();

    /// A handy singleton interface
    static ThreadPool& DefaultInstance();
private:
    struct Task
    {
        // Description about this task by Thread::StartRoutine.
        StartRoutine Routine;
        void* Context;
        unsigned long long Param; ///< any param

        // Description about this task by Closure.
        // If it's set, ignore the above routine.
        // Closure<void>* callback;

        // Description about this task by Function.
        // If it's set, ignore the above routine.
        // Function<void ()> function;

        void Run() const;
    };

    struct TaskNode : Task
    {
        list_node link;
    };

    struct PooledThread : public Thread
    {
        list_node link;
    };

    void AddTaskInternal(
        bool is_priority,
        StartRoutine routine,
        void* context,
        unsigned long long param,
        Closure<void>* callback,
        const Function<void ()>& function
    );

    void CommonConstruct(
        const std::string& name,
        int min_concurrencys,
        int max_concurrencys,
        int idle_timeout,
        size_t stack_size
    );

    bool NeedNewThread() const;
    void AddThread();
    bool DequeueTask(Task* task);
    void WorkerLoop();
    void WorkRoutine(PooledThread* concurrency);

private:
    std::string m_name;
    // Atomic<bool> m_Exit;
    // Atomic<bool> m_Terminating;
    bool m_Exit;
    bool m_Terminating;
    size_t m_MinThreads;
    size_t m_MaxThreads;
    int m_IdleTime;
    size_t m_StackSize;

    mutable Mutex m_Mutex;
    ConditionVar m_NewTaskCond; ///< to wakeup worker threads
    ConditionVar m_AllWorkersExitedCond; ///< will be signaled by the last worker thread
    std::list<PooledThread> m_Threads;
    std::list<PooledThread> m_FreedThreads;
    volatile size_t m_NumThreads;
    volatile size_t m_NumBusyThreads;
    std::list<TaskNode> m_PendingTasks;
    volatile size_t m_NumPendingTasks;
    std::list<TaskNode> m_FreeTasks;
    volatile size_t m_NumIdleTasks;
};


#endif // COMMON_THREAD_POOL_H_

