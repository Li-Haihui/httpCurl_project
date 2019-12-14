#ifndef _BASE_THREAD_H
#define _BASE_THREAD_H

class CBaseThread
{
    public:
        CBaseThread();
        virtual ~CBaseThread();
        bool startThread();
        bool bindTask(CBaseTask *task);
        
    private:
        pthread_t pthread;
        CBaseTask *task;
        
}

#endif

