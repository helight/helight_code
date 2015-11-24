// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-01-05
// Description:
//
#include <string>
#include <iostream>

extern void test();

int main(int argc, char **argv)
{
    std::string tt = "tree_log-00000000001.log";

    std::cout << "file index: " << tt.substr(9, 11) << std::endl;
    return 0;
}
