#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

#include <pthread.h>
#include <deque>

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
        std::deque<CBaseTask*> m_taskQueue;
         
};

#endif

