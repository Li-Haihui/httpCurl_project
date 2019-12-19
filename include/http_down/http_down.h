#ifndef _HTTP_DOWN_H
#define _HTTP_DOWN_H

#include <vector>

#include "down_loadTask.h"


class CHttpDown
{
    public:
        CHttpDown();
        virtual ~CHttpDown();
        void     setRetryTimes(int retry_times) { m_retry_times = retry_times; }
        int      setTimeout(long time_out = 0);
        int      setRequestUrl(const std::string& url);
        int      setDownloadFile(const std::string& file_name);
        int      doDownLoad();
    private:
        //std::deque<DownLoadData_s>* getDownLoadData();
        double   getDownloadFileSize();
        int      splitDownloadCount(double down_size);
        
    private:
        int          m_retry_times;
        int          m_thread_count;
        int          m_id;
        long         m_time_out;

        std::string  m_downfile_path;
        std::string  m_url;
        long         m_http_code;
        pthread_mutex_t m_mutex;
        std::vector<DownLoadData_s> m_downLoadDataVec;
        //CDownLoadTask *m_task;
};

#endif
