// Copyright 2012, Tencent Inc.
// Author: Xiaokang Liu <hsiaokangliu>

#include <stdlib.h>
#include <sys/types.h>
#include <iostream>
#include "cpu_usage.h"
#include "gflags/gflags.h"
#include "gtest/gtest.h"
#include <glog/logging.h>

DEFINE_int32(pid, 0, "process id");
DEFINE_int32(sample_period, 100, "sample period (ms)");

META_NODE_NAMESPACE_BEGIN

TEST(CPU, CpuRate)
{
    double cpu = 0;
    if (GetCpuUsageSinceLastCall(getpid(), &cpu))
    {
        std::cout << "1 current process cpu: " << cpu << std::endl;
    }
    else
    {
        std::cout << "1 Error current process cpu: " << cpu << std::endl;
    }
    cpu = 0;
    if (GetCpuUsageSinceLastCall(FLAGS_pid, &cpu))
    {
        std::cout << "2 cpu usage of process " << FLAGS_pid << ": " << cpu << std::endl;
    }
    else
    {
        std::cout << "2 Error cpu usage of process " << FLAGS_pid << ": " << cpu << std::endl;
    }

    cpu = 0;
    if (GetProcessCpuUsage(getpid(), FLAGS_sample_period, &cpu))
    {
        std::cout << "3 current process cpu=" << cpu << std::endl;
    }
    else
    {
        std::cout << "3 Error current process cpu: " << cpu << std::endl;
    }
    cpu = 0;
    if (GetProcessCpuUsage(FLAGS_pid, FLAGS_sample_period, &cpu))
    {
        std::cout << "4 cpu usage of process=" << FLAGS_pid << "cpu=" << cpu << std::endl;
    }
    else
    {
        std::cout << "4 Error cpu usage of process=" << FLAGS_pid << "cpu=" << cpu << std::endl;
    }
}

META_NODE_NAMESPACE_END

/*
int main(int argc, char **argv)
{
    FLAGS_log_dir = "../log/";
    FLAGS_logbufsecs = 0;
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging("meta_node_test");
    //
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
*/
