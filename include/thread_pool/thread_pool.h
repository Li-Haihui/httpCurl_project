#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "task_queue.h"

class CThreadPool
{
    public:
        CThreadPool(int num);
        virtual ~CThreadPool();
        bool start();
        bool addTask(CBaseTask *task);
        void stop();
    private:
        void run();
        static void* startThread(void *arg);

    private:
        pthread_t *m_pthreads;
        CTaskQueue m_taskQueue;
        int m_threadNum;
        int m_freethreadNum;
        pthread_mutex_t m_mutex;
        pthread_cond_t m_threadFullCond;
        pthread_cond_t m_taskQueueEmptyCond;
        
};

#endif
