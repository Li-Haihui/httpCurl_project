#include "task_queue.h"

CTaskQueue::CTaskQueue()
{
    int ret = 0;
    pthread_mutex_init(&m_mutex, NULL);
}

CTaskQueue::~CTaskQueue()
{
    pthread_mutex_lock(&m_mutex);
    if(!m_taskQueue.empty())
    {
        m_taskQueue.clear();
    }
    pthread_mutex_unlock(&m_mutex);
    
    pthread_mutex_destroy(&m_mutex);
}


bool CTaskQueue::push_back(CBaseTask *task)
{
    if(!task)
    {
        return false;
    }
    pthread_mutex_lock(&m_mutex);
    m_taskQueue.push_back(task);
    pthread_mutex_unlock(&m_mutex);

    return true;
}

CBaseTask* CTaskQueue::pop()
{
    CBaseTask *task = NULL;
    
    pthread_mutex_lock(&m_mutex);
    if(!m_taskQueue.empty())
    {
        task = m_taskQueue.pop();
    }
    pthread_mutex_unlock(&m_mutex);

    return task;
}


bool CTaskQueue::isEmpty()
{
    bool flag = false;
    
    pthread_mutex_lock(&m_mutex);
    if(!m_taskQueue.empty())
    {
        flag = true;
    }
    pthread_mutex_unlock(&m_mutex);

    return flag;
}

