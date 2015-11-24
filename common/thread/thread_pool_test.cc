// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#include "thread_pool.h"

TEST(ThreadPool, Attributes)
{
    ThreadPool threadpool;
    threadpool.SetMinThreads(1);
    EXPECT_EQ(1, threadpool.GetMinThreads());

    threadpool.SetMaxThreads(2);
    EXPECT_EQ(2, threadpool.GetMaxThreads());

    threadpool.SetMinThreads(0);
    EXPECT_EQ(0, threadpool.GetMinThreads());

    threadpool.SetMaxThreads(0);
    EXPECT_EQ(0, threadpool.GetMaxThreads());

    int num_cores = GetLogicalCpuNumber();
    threadpool.SetMinThreads(-1);
    EXPECT_EQ(num_cores, threadpool.GetMinThreads());

    threadpool.SetMaxThreads(-1);
    EXPECT_EQ(num_cores, threadpool.GetMaxThreads());
}

static void Nop(void*, unsigned long long)
{
}

static void test_blocking(void*, unsigned long long)
{
    ThisThread::Sleep(10);
}

static void test_nonblocking(void*, unsigned long long)
{
}

TEST(ThreadPool, MessTasks)
{
    ThreadPool threadpool(0, 4);
    for (int i = 0; i < 10000; ++i)
    {
        threadpool.AddTask(test_nonblocking);
    }
}

TEST(ThreadPool, Terminate)
{
    ThreadPool threadpool;
    for (int i = 0; i < 10000; ++i)
        threadpool.AddTask(Nop);
    threadpool.Terminate();
    threadpool.Terminate();
}

TEST(ThreadPool, AfterBusy)
{
    ThreadPool threadpool(0, 4);
    for (int i = 0; i < 1000; ++i)
    {
        threadpool.AddTask(test_nonblocking);
    }
    ThisThread::Sleep(10);
    for (int i = 0; i < 1000; ++i)
    {
        threadpool.AddTask(test_nonblocking);
    }
}

TEST(ThreadPool, MixedBlockingAndNonblocking)
{
    ThreadPool threadpool(0, 4);
    threadpool.AddTask(test_blocking);
    threadpool.AddTask(test_blocking);
    threadpool.AddTask(test_blocking);
    for (int i = 0; i < 10; ++i)
    {
        threadpool.AddTask(test_nonblocking);
    }
}

TEST(ThreadPool, Blocking)
{
    ThreadPool threadpool(0, 4);
    threadpool.AddTask(test_blocking);
    threadpool.AddTask(test_blocking);
    threadpool.AddTask(test_blocking);
    threadpool.AddTask(test_blocking);
}

static void test_sleep(void*, unsigned long long param)
{
    ThisThread::Sleep(1);
}

TEST(ThreadPool, SlowCall)
{
    ThreadPool threadpool;
    for (int i = 0; i < 10; ++i)
    {
        threadpool.AddTask(test_sleep, NULL, i);
    }
}

TEST(ThreadPool, GlobalFunction)
{
    ThreadPool threadpool(4, 4);
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            threadpool.AddTask(Nop, NULL, i*20+j);
            ThreadPool::Stats stats;
            threadpool.GetStats(&stats);
        }
    }
}

class Foo
{
public:
    void test1()
    {
    }
    void test2(intptr_t param)
    {
    }
};

TEST(ThreadPool, MemberFunction)
{
    ThreadPool threadpool(4, 4);
    Foo foo;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            threadpool.AddTask(MAKE_THREAD_CALLBACK(Foo, test1), &foo);
            threadpool.AddTask(
                MAKE_PARAMETERIZED_THREAD_CALLBACK(Foo, test2, intptr_t), &foo, i*20+j);
            ThreadPool::Stats stats;
            threadpool.GetStats(&stats);
        }
    }
}

TEST(ThreadPool, Closure)
{
    ThreadPool threadpool(4, 4);
    Foo foo;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            threadpool.AddTask(NewClosure(&foo, &Foo::test1));
            threadpool.AddTask(
                NewClosure(
                    &foo, &Foo::test2, static_cast<intptr_t>(i*20+j)));
            ThreadPool::Stats stats;
            threadpool.GetStats(&stats);
        }
    }
}

TEST(ThreadPool, Function)
{
    ThreadPool threadpool(4, 4);
    Foo foo;
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            threadpool.AddTask(NewClosure(&foo, &Foo::test1));
            threadpool.AddTask(
                Bind(&Foo::test2, &foo, static_cast<intptr_t>(i*20+j)));
            ThreadPool::Stats stats;
            threadpool.GetStats(&stats);
        }
    }
}

const int kLoopCount = 500000;

static void DoNothong(void* p, unsigned long long)
{
}

class ThreadPoolTest : public testing::TestWithParam<int> {};

TEST_P(ThreadPoolTest, Performance)
{
    int num_threads = GetParam();
    std::cout << "Test with " << num_threads << " threads." << "\n";
    ThreadPool threadpool(num_threads, num_threads);
    for (int i = 0; i < kLoopCount; ++i)
        threadpool.AddTask(DoNothong, NULL, 0);
    threadpool.WaitForIdle();
}

INSTANTIATE_TEST_CASE_P(ThreadPoolTest, ThreadPoolTest, testing::Values(1, 2, 4, 8));

static void LatencyProc(void* p, unsigned long long issue_time)
{
    int64_t handle_time = GetTimeStampInUs();
    int64_t latency = handle_time - issue_time;
    AtomicAdd(static_cast<int64_t*>(p), latency);
}

static void LatencyTest(int loop_count, bool slow_add)
{
    int64_t total_latency = 0;
    int64_t total_add_latency = 0;
    {
        ThreadPool threadpool;
        for (int i = 0; i < loop_count; ++i)
        {
            int64_t t0 = GetTimeStampInUs();
            threadpool.AddTask(LatencyProc, &total_latency, t0);
            int64_t t1 = GetTimeStampInUs();
            total_add_latency += t1 - t0;
            if (slow_add)
                ThisThread::Sleep(0);
        }
    }
    printf("average latency = %g\n", 1.0 * total_latency / loop_count);
    printf("average add latency = %g\n", 1.0 * total_add_latency / loop_count);
}

TEST(ThreadPool, FastAddLatency)
{
    LatencyTest(100000, false);
}

TEST(ThreadPool, SlowAddLatency)
{
    LatencyTest(100, true);
}

TEST(ThreadPool, CreateDestroyPerformance)
{
    for (int i = 0; i < 100; ++i)
    {
        ThreadPool threadpool;
        threadpool.AddTask(Nop);
        threadpool.Terminate(true);
    }
}

