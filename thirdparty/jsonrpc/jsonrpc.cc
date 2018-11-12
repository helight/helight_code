/*
 * jsonrpc.cc
 *
 *  Created on: 2010-11-25
 *      Author: laelli,helightxu
 * add get method
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>

#include "jsonrpc.h"

namespace json {
namespace rpc {

bool StringsAreEqual(const std::string& s1, const std::string& s2)
{
    std::string::const_iterator p1 = s1.begin();
    std::string::const_iterator p2 = s2.begin();
    std::string::const_iterator l1 = s1.end();
    std::string::const_iterator l2 = s2.end();

    while(p1 != l1 && p2 != l2)
    {
        if(std::toupper(*(p1++)) != std::toupper(*(p2++)))
            return false;
    }
    return (s2.size() == s1.size()) ? true : false;
}

std::string& replace_str(std::string& str,
                         const std::string& old_value,
                         const std::string& new_value)
{
    while (true)
    {
        std::string::size_type   pos(0);
        if((pos=str.find(old_value))!=std::string::npos)
            str.replace(pos,old_value.length(),new_value);
        else break;
    }

    return str;
}

void Container::Run(Handler& handler)
{
}

} //namespace json
} //namespace rpc
