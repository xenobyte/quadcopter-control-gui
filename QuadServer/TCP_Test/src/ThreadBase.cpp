
#include "TCP_TestConnection.h"
#include "ThreadBase.h"

#include <sched.h>
#include <signal.h>
#include <cassert>
#include <iostream>

static UnqLogger T_LOG {LOG.createLogger("ThreadBase")};

std::vector<ThreadBase*> THREADS;

void * ThreadBase::ThreadFunc(void *param)
{
    assert(param != 0);

    ThreadBase *pObj = static_cast<ThreadBase*>(param);

    T_LOG->info("Thread starting " + std::to_string(pthread_self()));

    // Base priority change later
    pObj->SetPriority(5);
    pObj->Run();
    T_LOG->info("Thread finished " + std::to_string(pthread_self()));
    pObj->CleanUp();

    return NULL;
}

ThreadBase::ThreadBase() :
    mThreadHdl(NULL)
{
}

ThreadBase::~ThreadBase(void)
{
}

void ThreadBase::SetPriority(int priority)
{
    struct sched_param sparam;
    sparam.sched_priority = priority;

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &sparam) == -1) {
        T_LOG->error("Can't change pthread priority");
    }
}

bool ThreadBase::Start(void)
{
    if (mThreadHdl)
        return false;
    mThreadHdl = new pthread_t;
    if (mThreadHdl == 0){
    	std::cout << "mThreadHdl == 0" << std::endl;
    }
    int ret = pthread_create(mThreadHdl,
                             NULL,
                             ThreadBase::ThreadFunc,
                             static_cast<void*>(this));

    if (ret) {
    	T_LOG->error("can't create thread");
        delete mThreadHdl;
        mThreadHdl = NULL;
        return false;
    }
    else {
        std::cout << "Thread push back" << std::endl;
    	THREADS.push_back(this);
        return true;
    }
    return true;
}

void ThreadBase::Stop(void)
{
    if (mThreadHdl != NULL) {
        pthread_cancel(*mThreadHdl);
        T_LOG->info("Thread stopped " + std::to_string(*mThreadHdl));
        delete mThreadHdl;
        mThreadHdl = NULL;
    }
    CleanUp();
}

bool ThreadBase::IsRunning(void)
{
    return mThreadHdl != NULL;
}
