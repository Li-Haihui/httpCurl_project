#include "base_task.h"

#include <iostream>

CBaseTask::CBaseTask(int taskID)
{
    m_taskID = taskID;
}

CBaseTask::~CBaseTask()
{

}

int CBaseTask::getTaskID()
{
    return m_taskID;
}

int CBaseTask::workTaskFun()
{
    std::cout<<"CBaseTask::workTaskFun:"<<getTaskID()<<std::endl;
    return 0;
}


