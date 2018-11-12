// Copyright (c) 2013, HelightXu
// Author:  Zhwen Xu<HelightXu@gmail.com>
// Created: 2013-12-16
// Description:
//

#include <arpa/inet.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <ifaddrs.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <map>
// #include <boost/regex.hpp>
#include "glog/logging.h"
#include "util.h"

int64_t CUtil::milli_seconds()
{
    struct timeval now;
    gettimeofday(&now, 0);

    return static_cast<uint64_t>(now.tv_sec * 1000)
        + static_cast<uint64_t>(now.tv_usec / 1000);
}

int32_t CUtil::seconds()
{
    struct timeval now;
    gettimeofday(&now, 0);

    return static_cast<uint32_t>(now.tv_sec);
}

std::string CUtil::get_local_ip(const char* dev)
{
    struct ifaddrs* addr_struct = NULL;
    struct ifaddrs* ifaddr= NULL;
    std::string local_ip = "";
    void* tmp_addr = NULL;

    if (0 != getifaddrs(&addr_struct))
    {
        VLOG(3) << "Get device ip error";
        return local_ip;
    }

    ifaddr =  addr_struct;
    while (addr_struct != NULL)
    {
        if (addr_struct->ifa_addr->sa_family == AF_INET)
        {
            //  is a valid IP4 Address
            tmp_addr = &((struct sockaddr_in*)addr_struct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmp_addr, addressBuffer, INET_ADDRSTRLEN);
            if (strcmp(addr_struct->ifa_name, dev) == 0)
            {
                local_ip = std::string(addressBuffer);
                break;
            }
        }
        else if (addr_struct->ifa_addr->sa_family == AF_INET6)
        {
            // is a valid IP6 Address
            tmp_addr = &((struct sockaddr_in *)addr_struct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmp_addr, addressBuffer, INET6_ADDRSTRLEN);
            if (strcmp(addr_struct->ifa_name, dev) == 0)
            {
                local_ip = std::string(addressBuffer);
                break;
            }
        }
        addr_struct = addr_struct->ifa_next;
    }

    if (ifaddr)
    {
        freeifaddrs(ifaddr);
    }

    return local_ip;
}

std::string CUtil::get_ip_by_dev(const char* dev)
{
    std::map<int,int>  m_ip_fd;   // 保存ip4和ip6的网络地址信息
    m_ip_fd[AF_INET] = socket(AF_INET, SOCK_DGRAM, 0);
    // m_ip_fd[AF_INET6] = socket(AF_INET6, SOCK_DGRAM, 0);
    std::map<int,int>::iterator it = m_ip_fd.begin();

    for (; it != m_ip_fd.end(); ++it)
    {
        int ip_type = it->first;
        int fd = it->second;

        int intrface;
        ifconf ifc;
        ifreq buf[32];  // 网卡信息数组(最多支持32个)

        if (fd <= 0)
        {
            LOG(ERROR) << "open socket error";
            continue;
        }
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = reinterpret_cast<caddr_t>(buf);

        if (0 != ioctl(fd, SIOCGIFCONF, reinterpret_cast<char*>(&ifc)))
        {
            LOG(ERROR) << "get ifconf info by ioctl error ";
            close (fd);
            continue;
        }
        intrface = ifc.ifc_len / sizeof(ifreq);
        while (intrface-- > 0)
        {
            if (0 != ioctl(fd, SIOCGIFADDR, reinterpret_cast<char*>(&buf[intrface])))
            {
                LOG(ERROR) << "get device info error";
                continue;
            }
            if (strcmp(buf[intrface].ifr_name, dev) == 0)
            {
                std::string ip;

                in_addr ip_address =
                    reinterpret_cast<sockaddr_in*>(&buf[intrface].ifr_addr)->sin_addr;
                ip = inet_ntop(ip_type, reinterpret_cast<void *>(&ip_address)
                               , const_cast<char*>(ip.data()), 32);
                close(fd);
                return ip;
            }
        }
        close (fd);
    }
    return "";
}

bool CUtil::regex(const std::string& str_txt, const std::string& pattern)
{
    /*
    boost::regex exp(pattern.c_str());
    boost::cmatch what;
    return boost::regex_match(str_txt.c_str(), what, exp);
    */
}

void CUtil::list(const std::string& dirpath,
    const std::string& pattern,
    std::vector<std::string>* file_names)
{
    if (NULL == file_names || pattern.empty())
    {
        LOG(ERROR) << "no file names vector or pattern is empty " << pattern;
        return;
    }

    DIR* dir = opendir(dirpath.c_str());
    if (NULL == dir || NULL == file_names)
    {
        LOG(ERROR) << "open dir error: " << errno << " path: " << dirpath;
        return;
    }

    // boost::regex reg(pattern);

    for (;;)
    {
        errno = 0;
        struct dirent* ent = readdir(dir);
        if (NULL == ent)
        {
            if (errno != 0)
            {
                int errcode = errno;
                if (EACCES == errcode)
                {
                    LOG(ERROR) << " Access denied: " << errno << " path: " << dirpath;
                    continue;
                }
                LOG(ERROR) << "read dir error: " << errno << " path: " << dirpath;
            }

            break; // over
        }

        if ((0 == strcmp(ent->d_name, "."))
         || (0 == strcmp(ent->d_name, "..")))
        {
            continue;
        }

        if (DT_REG == ent->d_type)
        {
            // boost::cmatch what;
            // if (boost::regex_match(ent->d_name, what, reg))
            // {
            //     file_names->push_back(ent->d_name);
            // }
            VLOG(4) << ent->d_name;
        }
    }

    closedir(dir);
}
