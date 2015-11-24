// Copyright (c) 2013, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2013-12-27
// Description:
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include "common/file/file.h"
#include "common/file/recordio.h"

class test
{
public:
    void init(uint32_t num)
    {
        _md5 = 444444444 + num;
        _data = 333333 + num;
        _size = 22222 + num;
        _rep = 11 + num;
    }

    uint8_t writeint32(uint32_t d, char *buf);
    uint8_t writeint64(uint64_t d, char *buf);
    uint32_t readint32(const char* buf);
    uint64_t readint64(const char* buf);
    uint32_t serialize_to_string(std::string& buf);
    void parse_from_string(const char* buf, uint32_t size);
    void print_value()
    {
        printf("md5: %ld, data: %d, rep: %d\r\n", _md5, _data, _rep);
    }

private:
    uint64_t _md5;
    uint32_t _data;
    uint16_t _size;
    uint8_t  _rep;
};

uint8_t test::writeint32(uint32_t d, char *buf)
{
    for (uint8_t i = 0; i < 4; ++i)
    {
        uint8_t c = (uint8_t)((d >>((3 - i)*8)) & 0xFF);
        buf[i] = c;
        printf("wbuf %02x\r\n", buf[i]);
    }
    return sizeof(uint32_t);
}

uint8_t test::writeint64(uint64_t d, char *buf)
{
    writeint32((uint32_t)(d>>32), buf);
    writeint32((uint32_t)(d), buf + 4);
    return sizeof(uint64_t);
}

uint32_t test::readint32(const char* buf)
{
    uint32_t val = 0;
    for (uint8_t i = 0 ; i < 4; ++i)
    {
        printf("rbuf %02x\r\n", *(buf + i));
        val <<= 8;
        val += (*(buf + i) & 0xFF);
    }
    return val;
}

uint64_t test::readint64(const char* buf)
{
    uint64_t val = 0;
    val = readint32(buf);
    val <<= 32;
    val += readint32(buf + 4);

    return val;
}

uint32_t test::serialize_to_string(std::string& buf)
{
    char buffer[256];
    uint8_t size = 0;
    size += writeint64(_md5, buffer + size);
    size += writeint32(_data, buffer + size);
    buffer[size] = _rep;
    ++size;
    buf = std::string(buffer, size);
    return size;
}

void test::parse_from_string(const char* buf, uint32_t size)
{
    _md5 = readint64(buf);
    _data = readint32(buf + 8);
    _rep =  *(buf + 12);
}

int main(int argc, char** argv)
{
    test tt;
    std::string buf;
    uint32_t count = 0;

    File *file = File::Open("test.log", "w+");
    RecordWriter *recordw = new RecordWriter(file);
    recordw->set_use_compression(false);

    for (uint8_t i = 0; i < 2; ++i) {
        tt.init(i);
        tt.print_value();
        count = tt.serialize_to_string(buf);
        recordw->WriteRecord(buf, count);
        // std::cout << "buf size: " << buf.size() << " buf: " << buf << std::endl;
    }

    file->Close();
    if (file) {
        delete file;
        file = NULL;
    }

    file = File::Open("test.log", "r");
    RecordReader *recordr = new RecordReader(file);
    std::string data;
    uint64_t size = 0;
    while (recordr->ReadRecord(data, &size)) {
        printf("read data: %s data.size: %ld size: %ld\r\n", data.data(),  data.size(), size);
        tt.parse_from_string(data.data(), size);
        tt.print_value();
        data.clear();
        size = 0;
    };
    file->Close();
    if (file) {
        delete file;
        file = NULL;
    }
    return 0;
}
