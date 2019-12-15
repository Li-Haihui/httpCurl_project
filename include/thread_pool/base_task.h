#ifndef _BASE_TASK_H
#define _BASE_TASK_H

class CBaseTask
{
    public:
        CBaseTask(int taskID);
        virtual ~CBaseTask(void);
        virtual int workTaskFun();
        int getTaskID();
    private:
        int m_taskID;
};

#endif
