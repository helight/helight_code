// Copyright (c) 2013, HelightXu
// Author:  Zhwen Xu<HelightXu@gmail.com>
// Created: 2013-12-17
// Description:
//
//
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <gtest/gtest.h>

#include "util.h"

TEST(CUtilTest, get_local_ip)
{
    std::string ip = CUtil::get_local_ip("eth0");
    std::cout << "Ip : " << ip << std::endl;
    // EXPECT_TRUE(ip.empty());
    ip = CUtil::get_local_ip("lo");
    EXPECT_STREQ(ip.c_str(), "127.0.0.1");
    std::cout << "Ip : " << ip << std::endl;
}

TEST(CUtilTest, get_ip_by_dev)
{
    std::string ip = CUtil::get_ip_by_dev("eth0");
    std::cout << "get_ip_by_dev Ip : " << ip << std::endl;
    EXPECT_TRUE(ip.empty());
    ip = CUtil::get_ip_by_dev("lo");
    EXPECT_STREQ("127.0.0.1", ip.c_str());
    std::cout << "get_ip_by_dev Ip lo: " << ip.c_str() << std::endl;
}

TEST(CUtilTest, regex)
{
    std::string pattern1 = "^tree_log-(\\d{10})-(\\d{5}).log";

    EXPECT_TRUE(CUtil::regex("tree_log-1392168455-00000.log", pattern1));
//    EXPECT_FALSE(CUtil::regex("tree_log-1392168455-000000.log", pattern));
//    EXPECT_FALSE(CUtil::regex("tree_log-1392168455-000000.log323", pattern));
//    EXPECT_FALSE(CUtil::regex("tree_log-1392168455-000000234.log", pattern));
//    EXPECT_FALSE(CUtil::regex("tree_log-139216845523423-000000.log", pattern));
//    EXPECT_FALSE(CUtil::regex("tree_log-1392168455--000000.log", pattern));
}
/*
TEST(CUtilTest, dir_list)
{
    std::vector<std::string> file_list;
    CUtil::list(FLAGS_tree_log_path, "^tree_log-(\\d{10})-(\\d{5}).log", &file_list);
    uint32_t size = file_list.size();
    for(uint32_t i = 0; i < size; ++i)
    {
        std::cout << "file: " << file_list[i] << std::endl;
    }
}
*/
class TestSerialize
{
public:
    void init(uint32_t num = 0)
    {
        _md5 = 0xf96b697d7cb7938dULL + num;
        _data = 3616572387 + num;
        _size = 65234 + num;
        _rep = 241 + num;
    }

    uint32_t serialize_to_string(char* buf);
    void parse_from_string(const char* buf, uint32_t size);
    void print_value()
    {
        printf("md5: %lu, data: %u, rep: %d\r\n", _md5, _data, _rep);
    }
    uint64_t get_md5(){ return _md5;}
    uint32_t get_data(){ return _data;}
    uint16_t get_size(){ return _size;}
    uint8_t get_rep(){ return _rep;}

private:
    uint64_t _md5;
    uint32_t _data;
    uint16_t _size;
    uint8_t  _rep;
};


uint32_t TestSerialize::serialize_to_string(char* buffer)
{
    // char buffer[256];
    uint8_t size = 0;
    size += CUtil::writeint64(_md5, buffer + size);
    size += CUtil::writeint32(_data, buffer + size);
    size += CUtil::writeint16(_size, buffer + size);
    size += CUtil::writeint8(_rep, buffer + size);
    // buf = std::string(buffer, size);
    return size;
}

void TestSerialize::parse_from_string(const char* buf, uint32_t size)
{
    _md5 = 0;
    _data = 0;
    _rep = 0;
    uint32_t rsize = 0;
    rsize += CUtil::readint64(buf, &_md5);
    rsize += CUtil::readint32(buf + rsize, &_data);
    rsize += CUtil::readint16(buf + rsize, &_size);
    rsize +=  CUtil::readint8(buf + rsize, &_rep);
    printf("size: %d rsize:%d \r\n", size, rsize);
}

uint32_t write_path(char* buf, std::string& path)
{
    uint8_t size = 0;
    size += CUtil::writeint16(static_cast<uint16_t>(path.size()), buf);
    memcpy(buf + size, path.data(), path.size());
    size += path.size();
    printf("write path: %s size: %d \r\n", path.data(), static_cast<int>(path.size()));
    return size;
}

uint32_t read_path(const char* buf, std::string& path)
{
    char buf_tmp[512];
    uint32_t size = 0;
    uint16_t len = 0;
    bzero(buf_tmp, sizeof(buf_tmp));

    size += CUtil::readint16(buf, &len);
    memcpy(buf_tmp, buf + size, len);
    printf("read path: %s size: %d \r\n", buf_tmp, len);
    path = std::string(buf_tmp, len);
    size += len;

    printf("read path: %s size: %d \r\n", path.c_str(), len);
    return size;
}

/*
TEST(CUtilTest, Serialize)
{
    TestSerialize tt;
    static char buf[2048];

    File *file = File::Open("test.log", "w+");
    RecordWriter *recordw = new RecordWriter(file);
    recordw->set_use_compression(false);

    for (uint8_t i = 0; i < 2; ++i)
    {
        bzero(buf, sizeof(buf));
        tt.init(i);
        tt.print_value();
        uint32_t len = 0;
        std::string path = "/data/test/tasdfas/tawerwe/tawe/";
        len = write_path(buf, path);
        len += tt.serialize_to_string(buf + len);

        recordw->WriteRecord(std::string(buf, len), len);
    }

    delete recordw;
    file->Close();
    if (file)
    {
        delete file;
        file = NULL;
    }
}
TEST(CUtilTest, Parse)
{
    TestSerialize tt;
    std::string buf;

    File *file = File::Open("test.log", "r");
    RecordReader *recordr = new RecordReader(file);
    std::string data;
    uint64_t size = 0;
    uint8_t num = 0;
    while (recordr->ReadRecord(data, &size))
    {
        printf("read data: %s data.size: %ld size: %ld\r\n", data.data(),  data.size(), size);
        std::string path;
        const char *rdata = data.data();
        uint32_t len = read_path(rdata, path);
        tt.parse_from_string(rdata + len, size - len);
        tt.print_value();
        EXPECT_EQ(0xf96b697d7cb7938dULL + num, tt.get_md5());
        EXPECT_EQ(3616572387 + num, tt.get_data());
        EXPECT_EQ(65234 + num, tt.get_size());
        EXPECT_EQ(241 + num, tt.get_rep());
        data.clear();
        size = 0;
        ++num;
    };
    EXPECT_EQ(2, num);

    delete recordr;
    file->Close();
    if (file)
    {
        delete file;
        file = NULL;
    }
}
*/
