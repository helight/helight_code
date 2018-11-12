// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-02-14
// Description:
//
//
#include <cstdlib>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <string>
#include <iostream>

boost::regex expression("^select ([a-zA-Z]*) from ([a-zA-Z]*)");

bool regex(const std::string& txt, const std::string& rex)
{
    boost::regex exp(rex);
    boost::cmatch what;
    return boost::regex_match(txt.c_str(), what, exp);
}

int main(int argc, char* argv[])
{
    std::string in;
    boost::cmatch what;
    std::cout << "enter test string" << std::endl;
    getline(std::cin,in);
    std::string exp = "^select-(\\d{3})-(\\d{3}).log";
    //exp = "^tree_log-\\d-\\d.log)";

    if (regex(in, exp))
    {
        std::cout << "match the regex" << std::endl;
    }
    else
    {
        std::cout << "not match the regex" << std::endl;
    }
    /*
    boost::regex expression(exp);
    if (boost::regex_match(in.c_str(), what, expression))
    {
        for (int i = 0; i < what.size(); ++i)
            std::cout << "str :" << what[i].str() << std::endl;
    }
    else
    {
        std::cout << "Error Input" << std::endl;
    }
    */

    return 0;
}
