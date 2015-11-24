// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-09
// Description:
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//global variables
pthread_cond_t      condA  = PTHREAD_COND_INITIALIZER;
pthread_cond_t      condB  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t     mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadA(void* parm)
{
    int i = 0, rValue, loopNum = 0;

    while (i < 5) {
        //do stuff
        printf("Hello %d\n", ++loopNum);
        //sleep(1);
        //signal condition of thread b
        rValue = pthread_cond_signal(&condB);
        //lock mutex
        rValue = pthread_mutex_lock(&mutex);
        //wait for turn
        printf("waite on A\n");
        while(pthread_cond_wait(&condA, &mutex) != 0);
        //unlock mutex
        rValue = pthread_mutex_unlock(&mutex);

        i++;
    }
}

void *threadB(void* param)
{
    int n = 0, rValue;

    while (n < 5) {
        //lock mutex
        rValue = pthread_mutex_lock(&mutex);
        printf("waite on B\n");
        //wait for turn
        while(pthread_cond_wait(&condB, &mutex) != 0);
        printf("Goodbye2\n");
        //unlock mutex
        rValue = pthread_mutex_unlock(&mutex);
        //do stuff
        printf("Goodbye3\n");
        //signal condition a
        rValue = pthread_cond_signal(&condA);
        n++;
    }
}

int main(int argc, char *argv[])
{
    //create our threads
    pthread_t a, b;

    pthread_create(&a, NULL, threadA, NULL);
    pthread_create(&b, NULL, threadB, NULL);

    pthread_join(a, NULL);
    pthread_join(b,NULL);
}

