#include "base_task.h"

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


