// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description:
//

#ifndef COMMON_THREAD_H_
#define COMMON_THREAD_H_

# include <pthread.h>

/// �����Ա�����õ�ͨ�õ��̺߳���
template<typename Class, void (Class::*Member)()>
void GenericMemberFunctionAdapter(void* context, unsigned long long param)
{
    (static_cast<Class*>(context)->*Member)();
}

/// �Ѵ�һ�����������Ա����תΪ��ͨ�����������ص���
/// @param Class ����
/// @param Member ��Ա������
/// @param ParamType ��Ա�����Ĳ������ͣ�ֻ����ָ����� intptr_t/uintptr_t
/// ������ڳ�Ա������@code
/// void Test::XxxThread(void* param);
/// @endcode
/// �� MAKE_MEMBER_CALLBACK(Test, XxxThread)���ȿ����ɿ������̵߳ĺ���ָ�룬����Ϊ@code
/// void (*)(void* object, void* param)
/// @endcode
/// ע�⿪ͷ����һ���������������� this ָ�롣
#define MAKE_PARAMETERIZED_THREAD_CALLBACK(Class, Member, ParamType) \
    &::GenericParamMemberFunctionAdapter<Class, ParamType, &Class::Member>
template<typename Class, typename ParamType, void (Class::*Member)(ParamType)>
void GenericParamMemberFunctionAdapter(void* context, unsigned long long param)
{
    (static_cast<Class*>(context)->*Member)(ParamType(param));
}

// Thread ��
// ���û�ֱ�Ӵ����̺߳���ʹ��
class Thread
{
    class Impl;
public:
    typedef void (*StartRoutine)(void* context, unsigned long long param);

    // Ĭ�Ϲ�����̶߳���
    // ����� Initialize ����� Start
    Thread();

    explicit Thread(
        StartRoutine start_routine,
        void* context = NULL,
        unsigned long long param = 0
    );

    // Entry��������Ҫȥ����
    virtual ~Thread();

    // ��ʼ���̶߳���
    void Initialize(StartRoutine start_routine, void* context = NULL, unsigned long long param = 0);

    // ֻ���� Start ֮ǰ����
    void SetStackSize(size_t size);

    // ������� Start ��Ż���������
    bool Start();

    // �ȴ��������е��߳̽���
    // ֻ���߳��Ѿ������ˣ���û�� Detach������ Join
    // ����߳��Ѿ��������������� true
    bool Join();

    // ��ʵ���̺߳��̶߳�����룬Detach ֮�󣬲�����ͨ���κκ������ʵ�ʵ���߳�
    void Detach();

    void SendStopRequest();
    bool IsStopRequested() const;
    bool StopAndWaitForExit();

    // �����߳��Ƿ��ڴ��
    bool IsAlive() const;

    // �����Ƿ���Զ��̵߳� Join
    bool IsJoinable() const;

    // ���ϵͳ�����߳� handle�����������뺬�壬ƽ̨���
    pthread_t GetHandle() const;

    // ����߳�ID������PID��������
    int GetId() const;

private:
    // Thread ����������� BaseThread �࣬BaseThread ���� override
    // Entry �����ķ�ʽʹ�ã����� Thread ����Ҫ֧�� Detach����˲��ٴ�
    // BaseThread ������Thread �౾����Ӧ����ֱ���Ժ���Ϊ������ʱ������ʹ�õġ�
    // ������������ﱨ��˵�����ô����ࡣ
    // ��������� BaseThread::Entry ǩ����һ����ʹ�ñ��������Լ�鵽���ִ���
    virtual void Entry() const {}

private:
    Impl* m_pimpl;
};


#endif // COMMON_THREAD_H_INCLUDED

