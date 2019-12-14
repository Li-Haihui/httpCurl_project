#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

#include <pthread.h>
#include <queue>

#include "base_task.h"

class CTaskQueue
{
    public:
        CTaskQueue();
        virtual ~CTaskQueue();
        CBaseTask* pop();
        bool push_back(CBaseTask *task);
        bool isEmpty();
        void clear();
    private:
        pthread_mutex_t m_mutex;
        std::queue<CBaseTask> m_taskQueue;
         
}

#endif

