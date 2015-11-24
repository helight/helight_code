// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-09
// Description:
//

#include <iostream>
#include <string>
#include "xtimer.h"
#include "gtest/gtest.h"

class TimerTaskTest : public testing::Test
{
public:
    void SetUp()
    {
        count = 0;
    }
    void EchoCallBack(uint64_t timer_id)
    {
        ++count;
        std::cout << "echo callback, timer: " <<
            timer_id << ", count: " << count << std::endl;
    }

protected:
    TimerTask timer_task;
    uint32_t count;
};


TEST_F(TimerTaskTest, EchoTimerTest)
{
    // 添加一个周期性定时器
    Callback cb1 = std::bind(&TimerTaskTest::EchoCallBack, this, std::placeholders::_1);
    uint64_t id1 = timer_task.AddOneshotTimer(500, cb1);
    uint64_t id2 = timer_task.AddPeriodTimer(100, cb1);
    usleep(10000000);

    timer_task.RemoveTimer(id2);
/*
    // 添加一个短期触发的一次性定时器
    TimerTask::Callback cb2 = std::bind(&TimerTaskTest::EchoCallBack, this, &n2);
    timer_task.AddOneshotTimer(14, cb2);

    // 添加一个短时间触发的周期性定时器
    TimerTask::Callback cb3 = std::bind(&TimerTaskTest::EchoCallBack, this, &n3);
    uint64_t id3 = timer_task.AddPeriodTimer(20, cb3);

    // 添加一个不可能触发的一次性定时器
    TimerTask::Callback cb4 = std::bind(&TimerTaskTest::EchoCallBack, this, &n4);
    uint64_t id4 = timer_task.AddOneshotTimer(1000000000000LL, cb4);
*/
    // 休息一秒，等待一次性定时器执行完毕
    usleep(10000);
}
