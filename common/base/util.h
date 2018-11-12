#ifndef IDATA_FACTORY_META_NODE_UTIL_H
#define IDATA_FACTORY_META_NODE_UTIL_H
#pragma once
#include <fcntl.h>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>

class CUtil
{
public:
    static int64_t milli_seconds();
    static int32_t seconds();

    static std::string get_local_ip(const char* dev);
    static std::string get_ip_by_dev(const char* dev);
    static bool regex(const std::string& str_txt, const std::string& pattern);
    static void list(const std::string& dirpath,
                     const std::string& pattern,
                     std::vector<std::string>* file_names);

    inline static uint8_t writeint8(const uint8_t d, char* buf)
    {
        buf[0] = (uint8_t)(d & 0xFF);

        return sizeof(d);
    }

    inline static uint8_t writeint16(const uint16_t d, char* buf)
    {
        union bytes {
            uint8_t b[2];
            int16_t all;
        } theBytes;

        theBytes.all = d;
        for (uint8_t i = 0; i < sizeof(d); ++i)
        {
            writeint8(theBytes.b[i], buf + i);
        }


        return sizeof(d);
    }

    inline static uint8_t writeint32(const uint32_t d, char* buf)
    {
        // printf("write val :%u \r\n", d);
        union bytes
        {
            uint8_t b[4];
            uint32_t all;
        } theBytes;

        theBytes.all = d;
        for (uint8_t i = 0; i < sizeof(d); ++i)
        {
            writeint8(theBytes.b[i], buf + i);
        }

        return sizeof(d);
    }

    inline static uint8_t writeint64(const uint64_t d, char* buf)
    {
        union bytes
        {
            uint8_t b[8];
            uint64_t all;
        } theBytes;

        theBytes.all = d;
        for (uint8_t i = 0; i < sizeof(d); ++i)
        {
            writeint8(theBytes.b[i], buf + i);
        }

        return sizeof(d);
    }

    inline static uint8_t readint8(const char* buf, uint8_t* val)
    {
        *val = (uint8_t)(*buf & 0xFF);

        return 1;
    }

    inline static uint16_t readint16(const char* buf, uint16_t* val)
    {
        union bytes {
            uint8_t b[2];
            int16_t all;
        } theBytes;

        for (uint8_t i = 0; i < sizeof(theBytes.all); ++i)
        {
            readint8(buf + i, &theBytes.b[i]);
        }
        *val = (uint16_t)theBytes.all;

        return sizeof(theBytes.all);
    }

    inline static uint32_t readint32(const char* buf, uint32_t* val)
    {
        union bytes
        {
            uint8_t b[4];
            uint32_t all;
        } theBytes;
        for (uint8_t i = 0; i < sizeof(theBytes.all); ++i)
        {
            readint8(buf + i, &theBytes.b[i]);
        }
        *val = (uint32_t)theBytes.all;

        // printf("read val :%u \r\n", *val);
        return sizeof(theBytes.all);
    }

    inline static uint64_t readint64(const char* buf, uint64_t* val)
    {
        union bytes
        {
            uint8_t b[8];
            uint64_t all;
        } theBytes;

        for (uint8_t i = 0; i < sizeof(theBytes.all); ++i)
        {
            readint8(buf + i, &theBytes.b[i]);
        }
        *val = (uint64_t)theBytes.all;

        return sizeof(theBytes.all);
    }
};

#endif // IDATA_FACTORY_META_NODE_UTIL_H
