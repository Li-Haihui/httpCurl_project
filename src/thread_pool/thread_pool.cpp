#include "thread_pool_1.h"

CThreadPool::CThreadPool(int threadNum)
{
    m_threadNum = threadNum;
    m_pthreads = new pthread_t[num];

    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_threadFullCond, NULL);
    pthread_cond_init(&m_taskQueueEmptyCond, NULL);
}

CThreadPool::~CThreadPool()
{
    pthread_cond_broadcast(&m_taskQueueEmptyCond);
    m_taskQueue.clear();
    for (int i = 0; i < m_threadNum; i++) 
    {
        pthread_join(pthreads[i], NULL);
    }

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_threadFullCond);
    pthread_cond_destroy(&m_taskQueueEmptyCond);
    delete pthreads;
}


bool CThreadPool::start()
{
    for(int i = 0; i < threadNum; i++)
    {
        pthread_create(&m_pthreads[i], NULL, threadpool_func, NULL);
    }   
}

bool CThreadPool::addTask(CBaseTask *task)
{
    if(task)
    {
        return false;
    }

    m_taskQueue.push_back(task);
    pthread_cond_broadcast(&m_taskQueueEmptyCond);
    
}

void *threadpool_func(void *arg)
{
    while(true)
    {
        pthread_mutex_lock(&m_mutex);
        if(m_taskQueue.isEmpty())
        {
            pthread_cond_wait(&m_taskQueueEmptyCond, &m_mutex);
        }

        CBaseTask *task = m_taskQueue.pop();
        int ret = task->workTaskFun();
        if(ret != 0)
        {
            m_taskQueue.push_back(task);
        }
        
        pthread_mutex_unlock(&m_mutex);
    }
}

void CThreadPool::stop()
{
    
}

