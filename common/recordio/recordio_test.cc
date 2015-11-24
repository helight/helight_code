// Copyright (c) 2013, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2013-12-30
// Description:
//
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include <gtest/gtest.h>
#include "file.h"
#include "recordio.h"

TEST(ForRecordWriterTest, WriterTest)
{
    File *file = File::Open("test.log", "w+");
    int i = 0;
    std::string content = "sdfsdfsfsddddddddddddddddddddddddddd";
    RecordWriter *recordw = new RecordWriter(file);
    recordw->set_use_compression(false);

    for (i = 0; i < 10; ++i)
    {
        content += boost::lexical_cast<std::string>(i);
        recordw->WriteRecord(content, content.size());
    }

    file->Close();
    if (file)
    {
        delete file;
        file = NULL;
    }
}

TEST(ForRecordWriterTest, ReadTest)
{
    // File *file = File::Open("test.log", "r");
    File *file = File::Open("test.log", "r");
    RecordReader *recordr = new RecordReader(file);

    // char *data = NULL;
    std::string data;
    uint64_t size = 0;
    while (recordr->ReadRecord(data, &size))
    {
        printf("data: %s data.size(): %ld, size: %ld\r\n", data.c_str(), data.size(), size);
        data.clear();
        size = 0;
    };

    file->Close();
    if (file)
    {
        delete file;
        file = NULL;
    }
}

int main(int argc, char **argv)
{
//    FLAGS_log_dir = boost::str(boost::format("%s/log") % utils::get_program_path());
//    FLAGS_log_dir = "./log/";
//    google::ParseCommandLineFlags(&argc, &argv, true);
//    google::InitGoogleLogging("xxxx_test");

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
