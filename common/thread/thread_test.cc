// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include "base_thread.h"

class ThreadTest: public ThreadBase
{
public:
	virtual void Entry();

private:
	int test;
};

void ThreadTest::Entry()
{
	while (IsAlive())
	{
		printf("ThreadTest is runing...pid:%d---ttid:%ld---self:0x%x\r\n",
						getpid(), syscall(SYS_gettid), (unsigned int)pthread_self());
		Sleep(1000);
	};
}

int main(int argc, char* argv[])
{
	int num = 3;
	ThreadTest testhread[num];
	for (int i = 0; i < num; i++)
		testhread[i].Start();

	while (1)
	{
		printf("main loop ...... pid:%d---ttid:%ld---self:0x%x\r\n",
						getpid(), syscall(SYS_gettid), (unsigned int)pthread_self());
		sleep(10);
	};
}
