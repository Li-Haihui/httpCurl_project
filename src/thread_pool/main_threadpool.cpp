#include <iostream>
#include <unistd.h>

#include "work_task.h"
#include "thread_pool.h"

int test_ThreadPoolmain()
{
    CThreadPool threadpool(5);
    threadpool.start();
    
    for(int i = 0; i < 20; i++)
    {
        CBaseTask *baseTask = new CWorkTask(i+10);
        threadpool.addTask(baseTask);
    }
    
    //sleep(5);
    return 0;
}
