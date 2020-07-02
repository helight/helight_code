xlight
======

xlight

Code Style For C++ On This Code
=================
This doc just for this project, desc the C++ code style

Head
-----------------
### Head desc
``` c
// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-01-01
// Description:
//
```

### Head define
``` c
#ifndef FOO_BAR_BAZ_H_
#define FOO_BAR_BAZ_H_

...

#endif  // FOO_BAR_BAZ_H_
```
### Head include
1. C system files.
2. C++ system files.
3. Other libraries .h files.
4. Your project .h files.

### Class struct enum define
1. class name begin captal lettel, like Cxxx, NameServer
2. struct begin St, like Stxxx, StFileInfo
3. enum begin En, like Enxxx, EnTreeCode
4. member for class start _, like _size, _len, struct dont
5. member of enum all capital letter
6. method for class are capital first letter without '_' like GetFileInfo

``` c
 class BaseThread
 {
 public:
     BaseThread();
     ~BaseThread();

 private:
     bool _alive;
     uint8_t _thread_count;
 };

 struct StFileInfo
 {
     std::string _name;
     uint32_t _size;
 };

 enum EnFileOpCode
 {
     OPRET_OK = 0,
     OPRET_NOT_EXIST = 1000,
     OPRET_EXIST = 1001
 };
```
### for while if else

1. Has space between key words and "(" ,between operator and val
2. {} has single line

``` c
for (uint8_t i = 0; i < 10; ++i)
{
    ......
}

while (_stop)
{
    ......
};

if (_test)
{
    ......
}
else
{
    ......
}
```
### Function define
 1.For short
