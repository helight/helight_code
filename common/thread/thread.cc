// Copyright (c) 2014, HelightXu
// Author: Zhwen Xu<HelightXu@gmail.com>
// Created: 2014-03-08
// Description: Thread implementation
//

#include "thread.h"
#include "thread_base.h"

class Thread::Impl : public ::BaseThread
{
public:
    Impl():
        m_StartRoutine(NULL),
        m_Context(NULL),
        m_Param(0),
        m_closure(NULL)
    {
    }

    explicit Impl(
        StartRoutine start_routine,
        void* context,
        unsigned long long param
    ):
        m_StartRoutine(start_routine),
        m_Context(context),
        m_Param(param),
    {
    }


    ~Impl()
    {
        delete m_closure;

        // Reset all to null for debug purpose
        m_StartRoutine = NULL;
        m_Context = NULL;
        m_Param = 0;
    }

    void Initialize(StartRoutine start_routine, void* context, unsigned long long param)
    {
        CheckNotJoinable();

        m_StartRoutine = start_routine;
        m_Context = context;
        m_Param = param;

    }

    bool IsInitialized() const
    {
        return m_StartRoutine != NULL;
    }

private:
    virtual void Entry()
    {
        if (m_function) {
            m_function();
        }
    }

    virtual void OnExit()
    {
        if (!IsJoinable())
            delete this;
        else
            ThreadBase::OnExit();
    }

    void CheckNotJoinable() const
    {
        // Can't reinitialze a joinable thread
        if (IsJoinable())
            CHECK_ERRNO_ERROR(EINVAL);
    }

private:
    // Description about the routine.
    StartRoutine m_StartRoutine;
    void* m_Context;
    unsigned long long m_Param;
};


Thread::Thread(): m_pimpl(new Impl())
{
}

Thread::Thread(
    StartRoutine start_routine,
    void* context,
    unsigned long long param
) : m_pimpl(new Impl(start_routine, context, param))
{
}

Thread::~Thread()
{
    delete m_pimpl;
    m_pimpl = NULL;
}

void Thread::Initialize(
    StartRoutine start_routine,
    void* context,
    unsigned long long param)
{
    m_pimpl->Initialize(start_routine, context, param);
}

void Thread::SetStackSize(size_t size)
{
    return m_pimpl->SetStackSize(size);
}

bool Thread::Start()
{
    if (!m_pimpl->IsInitialized())
        CHECK_ERRNO_ERROR(EINVAL);
    return m_pimpl->Start();
}

bool Thread::Join()
{
    return m_pimpl->Join();
}

void Thread::Detach()
{
    // After detached, the m_pimpl will be deleted in Thread::Impl::OnExit.
    // So don't delete it here, just set it be NULL to mark this thread object
    // to be detached.
    m_pimpl->DoDetach();
    m_pimpl = NULL;
}

void Thread::SendStopRequest()
{
    m_pimpl->SendStopRequest();
}

bool Thread::IsStopRequested() const
{
    return m_pimpl->IsStopRequested();
}

bool Thread::StopAndWaitForExit()
{
    return m_pimpl->StopAndWaitForExit();
}

bool Thread::IsAlive() const
{
    return m_pimpl && m_pimpl->IsAlive();
}

bool Thread::IsJoinable() const
{
    return m_pimpl && m_pimpl->IsJoinable();
}

pthread_t Thread::GetHandle() const
{
    return m_pimpl->GetHandle();
}

int Thread::GetId() const
{
    return m_pimpl->GetId();
}

