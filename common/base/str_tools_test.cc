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

#include "str_tools.h"

TEST(StrToolsTest, split_string)
{
    std::string path = "/home////data/test/123/";
    std::vector<std::string> result;
    StrTools::split_string(path, '/', &result);

    EXPECT_EQ((unsigned int)4, result.size());
    ASSERT_STREQ("home", result[0].c_str());
    EXPECT_STRNE("home2", result[0].c_str());
    EXPECT_STREQ("data", result[1].c_str());
    EXPECT_STREQ("test", result[2].c_str());
    EXPECT_STREQ("123", result[3].c_str());

    path = "/home/data/test/123";
    result.clear();
    StrTools::split_string(path, '/', &result);
    EXPECT_EQ((unsigned int)4, result.size());
    ASSERT_STREQ("home", result[0].c_str());
    EXPECT_STRNE("home2", result[0].c_str());
    EXPECT_STREQ("data", result[1].c_str());
    EXPECT_STREQ("test", result[2].c_str());
    EXPECT_STREQ("123", result[3].c_str());
    path = "/";
    result.clear();
    StrTools::split_string(path, '/', &result);
    EXPECT_EQ((unsigned int)0, result.size());
}
