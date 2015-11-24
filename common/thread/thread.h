// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#ifndef COMMON_THREAD_H_
#define COMMON_THREAD_H_

# include <pthread.h>

/// 适配成员函数用的通用的线程函数
template<typename Class, void (Class::*Member)()>
void GenericMemberFunctionAdapter(void* context, unsigned long long param)
{
    (static_cast<Class*>(context)->*Member)();
}

/// 把带一个参数的类成员函数转为普通函数用来做回调。
/// @param Class 类名
/// @param Member 成员函数名
/// @param ParamType 成员函数的参数类型，只能是指针或者 intptr_t/uintptr_t
/// 例如存在成员函数：@code
/// void Test::XxxThread(void* param);
/// @endcode
/// 用 MAKE_MEMBER_CALLBACK(Test, XxxThread)，既可生成可用于线程的函数指针，类型为@code
/// void (*)(void* object, void* param)
/// @endcode
/// 注意开头多了一个参数，用来传递 this 指针。
#define MAKE_PARAMETERIZED_THREAD_CALLBACK(Class, Member, ParamType) \
    &::GenericParamMemberFunctionAdapter<Class, ParamType, &Class::Member>
template<typename Class, typename ParamType, void (Class::*Member)(ParamType)>
void GenericParamMemberFunctionAdapter(void* context, unsigned long long param)
{
    (static_cast<Class*>(context)->*Member)(ParamType(param));
}

// Thread 类
// 由用户直接传入线程函数使用
class Thread
{
    class Impl;
public:
    typedef void (*StartRoutine)(void* context, unsigned long long param);

    // 默认构造的线程对象
    // 必须调 Initialize 后才能 Start
    Thread();

    explicit Thread(
        StartRoutine start_routine,
        void* context = NULL,
        unsigned long long param = 0
    );

    // Entry，将来需要去掉。
    virtual ~Thread();

    // 初始化线程对象
    void Initialize(StartRoutine start_routine, void* context = NULL, unsigned long long param = 0);

    // 只能在 Start 之前调用
    void SetStackSize(size_t size);

    // 必须调用 Start 后才会真正启动
    bool Start();

    // 等待正在运行的线程结束
    // 只有线程已经运行了，且没有 Detach，才能 Join
    // 如果线程已经结束，立即返回 true
    bool Join();

    // 把实际线程和线程对象分离，Detach 之后，不能再通过任何函数访问到实际线程
    void Detach();

    void SendStopRequest();
    bool IsStopRequested() const;
    bool StopAndWaitForExit();

    // 返回线程是否还在存活
    bool IsAlive() const;

    // 返回是否可以对线程调 Join
    bool IsJoinable() const;

    // 获得系统级的线程 handle，具体类型与含义，平台相关
    pthread_t GetHandle() const;

    // 获得线程ID（类似PID的整数）
    int GetId() const;

private:
    // Thread 类起初派生自 BaseThread 类，BaseThread 是以 override
    // Entry 函数的方式使用，但是 Thread 类需要支持 Detach，因此不再从
    // BaseThread 派生，Thread 类本来就应该是直接以函数为参数来时初化来使用的。
    // 如果你遇到这里报错，说明你用错了类。
    // 这里故意与 BaseThread::Entry 签名不一样，使得编译器可以检查到这种错误。
    virtual void Entry() const {}

private:
    Impl* m_pimpl;
};


#endif // COMMON_THREAD_H_INCLUDED

