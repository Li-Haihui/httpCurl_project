#ifndef _DOWN_LOADTASK_H
#define _DOWN_LOADTASK_H

//#include <curl/curl.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include<sstream>



#include "base_task.h"
#include "curl.h"


typedef struct tagDownLoadData
{
    long        _startidx;
    long        _curridx;
    long        _endidx;
    char        *data;
    
    tagDownLoadData()
    {
        _startidx = 0;
        _curridx = 0;
        _endidx = 0;
        data = NULL;
    }
    
}DownLoadData_s;

class CDownLoadTask:public CBaseTask
{
    public:
        CDownLoadTask();
        ~CDownLoadTask();
        void      setRequestUrl(const std::string& url);
        void      setReqId(int reqId = 0);
        void      setTimeout(long timeOut = 0);
        void      setDownLoadType(int       downLoadType = 0);
        void      setDownLoadTaskData(DownLoadData_s *downLoadData);
        void      setRetryTimes(int retryTimes = 1);
        virtual int workTaskFun();

   private:
        static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
        
   private:
        int m_downLoadType;
        long m_timeOut;
        int m_retrytimes;
        int m_reqId;
        std::string  m_url;
        DownLoadData_s *m_downLoadData;
        
};

#endif
