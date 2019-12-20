#ifndef _HTTP_DOWN_H
#define _HTTP_DOWN_H

#include <vector>

#include "down_loadTask.h"


class CHttpDown
{
    public:
        CHttpDown();
        virtual ~CHttpDown();
        void     setRetryTimes(int retryTimes) { m_retryTimes = retryTimes; }
        int      setTimeout(long timeOut = 0);
        int      setRequestUrl(const std::string& url);
        int      setDownloadFile(const std::string& fileName);
        int      doDownLoad();
    private:
        //std::deque<DownLoadData_s>* getDownLoadData();
        double   getDownloadFileSize();
        int      splitDownloadCount(double downSize);
        
    private:
        int          m_retryTimes;
        int          m_threadCount;
        int          m_id;
        long         m_timeOut;

        std::string  m_downfilePath;
        std::string  m_url;
        long         m_httpCode;
        pthread_mutex_t m_mutex;
        std::vector<DownLoadData_s> m_downLoadDataVec;
        //CDownLoadTask *m_task;
};

#endif
