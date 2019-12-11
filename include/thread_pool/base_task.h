#ifndef _BASE_TASK_H
#define _BASE_TASK_H

class CBaseTask
{
    public:
        CBaseTask(int taskID);
        virtual ~CBaseTask();
        virtual void workTaskFun(void *data)=0;
        int getTaskID();
    private:
        int m_taskID;
}

#endif
