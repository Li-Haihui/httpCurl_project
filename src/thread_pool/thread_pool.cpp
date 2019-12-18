#include "thread_pool.h"

#include <iostream>


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
		while(m_taskQueue.empty() && !m_poolClose)
        {
            pthread_cond_wait(&m_taskQueueEmptyCond, &m_mutex);
        }
		
		if(m_poolClose)
		{
			pthread_mutex_unlock(&m_mutex);
			pthread_exit(NULL);
		}
        
        CBaseTask *task = NULL;
		if(!m_taskQueue.empty())
        {

			task = m_taskQueue.front();
			m_taskQueue.pop_front();
        }
		
		if(m_taskQueue.empty())
		{
			pthread_cond_signal(&m_threadNotEmptyCond);
		}

        if(task)
        {
            int ret = task->workTaskFun();
        }
		
        pthread_mutex_unlock(&m_mutex);
       
    }
}


CThreadPool::CThreadPool(int threadNum)
{
    m_threadNum = threadNum;
    m_pthreads = new pthread_t[threadNum];
	m_poolClose = 0;
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_taskQueueEmptyCond, NULL);
	pthread_cond_init(&m_threadNotEmptyCond, NULL);
}

CThreadPool::~CThreadPool()
{
	pthread_mutex_lock(&m_mutex);
	if(m_poolClose)
	{
		pthread_mutex_unlock(&m_mutex);
		return;
	}
	
	m_poolClose = 1;
	while(!m_taskQueue.empty())
	{
		pthread_cond_wait(&m_threadNotEmptyCond, &m_mutex);
	}
	
	m_poolClose = 1;

    pthread_cond_broadcast(&m_taskQueueEmptyCond);

	pthread_mutex_unlock(&m_mutex);
    for (int i = 0; i < m_threadNum; i++) 
    {
        pthread_join(m_pthreads[i], NULL);
    }

    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_taskQueueEmptyCond);
    pthread_cond_destroy(&m_threadNotEmptyCond);
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
	if(!m_taskQueue.empty())
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

