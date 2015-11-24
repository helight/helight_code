// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-09
// Description:
//

#include <pthread.h>
#include <unistd.h>
#include "condition_var.h"
#include "gtest/gtest.h"

ConditionVar cond;
Mutex mutex;

void *threadB(void* param)
{
    int n = 0, rValue;

    while (n < 5) {
        sleep(1);
        printf("waite on B\n");
        //wait for turn
        while(cond.Wait(&mutex, 1000) != 0);
        printf("Goodbye2\n");
        n++;
    }
}

TEST(condition_var, time_wait)
{
    //create our threads
    pthread_t b;
    pthread_create(&b, NULL, threadB, NULL);

    pthread_join(b,NULL);
}
