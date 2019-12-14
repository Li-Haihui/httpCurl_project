#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <pthread.h>

#include "task_queue.h"

class CThreadPool
{
    public:
        CThreadPool(int threadNum);
        virtual ~CThreadPool();
        bool addTask(CBaseTask *task);
        bool run();
    private:
        pthread_t m_pthreads;
        CTaskQueue m_taskQueue;
        int m_threadNum;
        int m_freethreadNum;
        pthread_mutex_t m_mutex;
        pthread_cond_t m_threadFullCond;
        //pthread_cond_t m_taskQueueNotEmptyCond;
        pthread_cond_t m_taskQueueEmptyCond;
}

#endif
