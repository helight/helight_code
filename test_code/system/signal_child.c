// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-05-02
// Description:
//
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void child_exit_handler(int signum)
{
    if(signum == SIGCHLD)
    {
        int status;
        int pid = wait(&status);
        printf("Child exit. %d status: %d\n", pid, status);
    }
}

int main()
{
    int pid;
    int i = 0;

    //想内核注册，处理 SIGCHLD信号的方式
    signal(SIGCHLD, child_exit_handler);

    if((pid = fork()) < 0)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);

    }else if(pid == 0){

        for(i = 0;i < 5;i ++)
        {
            printf("child loop.\n");
            sleep(1);
        }

    }else{

        for(i = 0;i < 5;i ++)
        {
            printf("Father loop.\n");
            sleep(2);
        }

    }

    exit(EXIT_SUCCESS);
}
