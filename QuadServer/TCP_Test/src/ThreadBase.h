
#ifndef THREADBASE_H_QWZX93J7
#define THREADBASE_H_QWZX93J7

#include <pthread.h>
#include <vector>
#include <iostream>

class ThreadBase
{
public:
    ThreadBase ();
    virtual ~ThreadBase ();

    void SetPriority(int prio);

    bool Start(void);
//    void *Join(void);
    void Stop(void);

    bool IsRunning(void);
    virtual void CleanUp(void) = 0;
    virtual void Run(void) = 0;


protected:
    // Tors

private:

    static void * ThreadFunc(void *param);

    pthread_t *mThreadHdl;
};

extern std::vector<ThreadBase*> THREADS;

#endif /* end of include guard: THREADBASE_H_QWZX93J7 */

