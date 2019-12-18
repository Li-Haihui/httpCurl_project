#include "work_task.h"

#include <iostream>

CWorkTask::CWorkTask(int taskID):CBaseTask(taskID)
{

}

CWorkTask::~CWorkTask()
{

}

int CWorkTask::workTaskFun()
{
    std::cout<<"CWorkTask::workTaskFun:"<<getTaskID()<<std::endl;
    return 0;
}
