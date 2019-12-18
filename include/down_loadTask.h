#ifndef _DOWN_LOADTASK_H
#define _DOWN_LOADTASK_H

typedef struct tagDownLoadData
{
    //FILE        *fp;
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
        void     SetRequestId(int id) { m_id = id;  }
        int      SetTimeout(long time_out = 0);
        int      SetRequestUrl(const std::string& url);
        int      SetRequestProxy(const std::string& proxy, long proxy_port);

        void     SetUserData(void *userdata) { m_userdata = userdata; }
        int      SetProgressCallback(ProgressCallback pc);
        int      SetResultCallback(ResultCallback rc);
        int      SetDownloadFile(const std::string& file_name);

        int      Perform();
        
        int      GetHttpCode() { return m_http_code; }
        bool     GetHeader(std::string* header);
        bool     GetErrorString(std::string* error_string);
        bool     SelfClose(void) { return m_close_self; }
        void*    GetUserData(void) { return m_userdata; }

        std::deque<DownLoadData_s>* getDownLoadData();

        int      DoDownLoad();

    protected:
        int      DownloadDefaultCallback(double total_size, double downloaded_size, void* userdata);
        void     ResultDefaultCallback(int id, bool success, const std::string& data);
        double   GetDownloadFileSize();
        //int      DoDownload(ThreadChunk* thread_chunk);
        int      SplitDownloadCount(double down_size);
        void  Reset(void);

    private:

        int          m_retry_times;
        int          m_thread_count;
        int          m_id;
        long         m_time_out;

        std::string  m_file_path;
        std::string  m_url;
        std::string  m_http_proxy;
        std::string  m_receive_header;
        std::string  m_error_string;

        bool         m_close_self;
        bool         m_multi_download;
        bool         m_download_fail;
        bool         m_is_running;
        bool         m_is_cancel;
        void*        m_userdata;
        long         m_http_code;
        long         m_proxy_port;
        double       m_total_size;
        double       m_downloaded_size;

        pthread_mutex_t m_mutex;

        //std::shared_ptr<HttpLock> m_httplock;
        ProgressCallback  m_download_callback;
        ResultCallback    m_result_callback;

        std::deque<DownLoadData_s> m_downLoadDataDeq;
        CDownLoadTask m_downLoadTask;

        
        
};


class CDownLoadTask:public CBaseTask
{
    public:
        CDownLoadTask();
        ~CDownLoadTask();
        int      SetRequestUrl(const std::string& url);
        int      SetTimeout(long time_out = 0);
        int      SetDownLoadType(int       downLoadType= 0);
        int      setDownLoadTaskData(DownLoadData_s *dataLoadData);
        void     SetRetryTimes(int retry_times) { m_retry_times = retry_times; }
        virtual int workTaskFun();

   private:
        static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
        
   private:
        std::string  m_url;
        int m_downLoadType;
        long m_timeOut;
        DownLoadData_s *m_dataLoadData;
        int m_retrytimes;
};

#endif
