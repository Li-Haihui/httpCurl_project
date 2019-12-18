#ifndef _HTTP_DOWN_H
#define _HTTP_DOWN_H

typedef struct tagDownLoadData
{
    long        _startidx;
    long        _endidx;
    void        *data;
    
}DownLoadData_s;


class CHttpDown
{
    public:
        CHttpDown();
        virtual ~CHttpDown();
        void     SetRetryTimes(int retry_times) { m_retry_times = retry_times; }
        int      SetTimeout(long time_out = 0);
        int      SetRequestUrl(const std::string& url);
        int      SetDownloadFile(const std::string& file_name);
        int      DoDownLoad();
    private:
        std::deque<DownLoadData_s>* getDownLoadData();
        int      SplitDownloadCount(double down_size);
        
    private:
        int          m_retry_times;
        int          m_thread_count;
        int          m_id;
        long         m_time_out;

        std::string  m_downfile_path;
        std::string  m_url;
        long         m_http_code;
        pthread_mutex_t m_mutex;
        vector<DownLoadData_s> m_downLoadDataDeq;
        CDownLoadTask *m_task;
};

#endif
