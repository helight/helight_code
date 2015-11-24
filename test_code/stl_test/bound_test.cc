// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-02-27
// Description:
//
#include <stdint.h>
#include <iostream>     // std::cout
#include <algorithm>    // std::lower_bound, std::upper_bound, std::sort
#include <vector>       // std::vector

#include <google/profiler.h>

void func1() {
    int i = 0;
    while (i < 100000) {
        ++i;
    }
}
void func2() {
    int i = 0;
    while (i < 200000) {
        ++i;
    }
}
void func3() {
    for (int i = 0; i < 1000; ++i) {
        func1();
        func2();
    }
}

int main () {
    ProfilerStart("my.prof");
    int myints[] = {10,20,30,30,20,10,10,20};
    std::vector<int> v(myints,myints+8);           // 10 20 30 30 20 10 10 20

    std::sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30

    std::vector<int>::iterator low,up;
    low = std::lower_bound (v.begin(), v.end(), 20); //
    up = std::upper_bound (v.begin(), v.end(), 20); //

    std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
    std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

    low = std::lower_bound (v.begin(), v.end(), 15); //
    up = std::upper_bound (v.begin(), v.end(), 25); //
    std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
    std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

    low = std::lower_bound (v.begin(), v.end(), 5); //
    up = std::upper_bound (v.begin(), v.end(), 35); //
    std::cout << "lower_bound at position " << (low- v.begin()) << '\n';
    std::cout << "upper_bound at position " << (up - v.begin()) << '\n';

    std::vector<uint32_t> vtest;
    vtest.resize(10);

    for (uint32_t i = 4; i < 10; ++i)
    {
        vtest[i] = i;
    }

    for (uint32_t i = 0; i < 10; ++i)
    {
        std::cout << "vector: " << i << " " << vtest[i] << std::endl;
    }

    func3();

    ProfilerStop();
    return 0;
}
