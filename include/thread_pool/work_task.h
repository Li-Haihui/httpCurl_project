#ifndef _WORK_TASK_H
#define _WORK_TASK_H

#include "base_task.h"

class CWorkTask:public CBaseTask
{
    public:
        CWorkTask(int taskID);
        virtual ~CWorkTask();
        virtual int workTaskFun();

};

#endif
