#include "thread_pool.h"


void* CThreadPool::startThread(void *arg)
{
    CThreadPool* threadpool = static_cast<CThreadPool*>(arg);
    threadpool->run();
    return NULL;
}

void CThreadPool::run()
{
    while(true)
    {
        pthread_mutex_lock(&m_mutex);
        while(m_taskQueue.isEmpty())
        {
            pthread_cond_wait(&m_taskQueueEmptyCond, &m_mutex);
        }
        
        CBaseTask *task = NULL;
        if(!m_taskQueue.isEmpty())
        {
            task = m_taskQueue.pop();
        }

        pthread_mutex_unlock(&m_mutex);

        if(task)
        {
            int ret = task->workTaskFun();
        }
        
       
    }
}


CThreadPool::CThreadPool(int threadNum)
{
    m_threadNum = threadNum;
    m_pthreads = new pthread_t[threadNum];

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
        pthread_join(m_pthreads[i], NULL);
    }

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_threadFullCond);
    pthread_cond_destroy(&m_taskQueueEmptyCond);
    delete m_pthreads;
}


bool CThreadPool::start()
{
    for(int i = 0; i < m_threadNum; i++)
    {
        pthread_create(&m_pthreads[i], NULL, &startThread, this);
    }

    return true;
}

bool CThreadPool::addTask(CBaseTask *task)
{
    if(!task)
    {
        return false;
    }

    
    pthread_mutex_lock(&m_mutex);
    if(!m_taskQueue.isEmpty())
    {
        m_taskQueue.push_back(task);
        pthread_cond_broadcast(&m_taskQueueEmptyCond);
    }
    else
    {
        m_taskQueue.push_back(task);
    }

    pthread_mutex_unlock(&m_mutex);
    return true;
    
}



void CThreadPool::stop()
{
    
}

