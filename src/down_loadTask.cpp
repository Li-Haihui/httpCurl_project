#include "down_loadTask.h"

CHttpDown::CHttpDown()
    : m_close_self(false)
    , m_retry_times(3)
    , m_http_code(0)
    , m_time_out(0)
    , m_proxy_port(0)
    , m_total_size(0.0)
    , m_downloaded_size(0.0)
    , m_download_fail(true)
    , m_is_running(false)
    , m_mutex(PTHREAD_MUTEX_INITIALIZER)
    , m_userdata(NULL)
{

}

CHttpDown::~CHttpDown()
{

}

int CHttpDown::SetRequestUrl(const std::string & url)
{
    m_url = url;
    return 0;
}

int CHttpDown::SetRequestProxy(const std::string& proxy, long proxy_port)
{
    m_http_proxy = proxy;
    m_proxy_port = proxy_port;

    return 0;
}



int CHttpDown::SetTimeout(long time_out)
{
    m_time_out = time_out;
    return 0;
}

int CHttpDown::SetDownloadFile(const std::string & file_name)
{
    m_file_path = file_name;
    return 0;
}

double CHttpDown::GetDownloadFileSize()
{
    if (m_url.empty())
    {
        return -1.0;
    }
    else
    {
        double down_file_length = -1.0;
        CURL *handle = curl_easy_init();

        if (handle)
        {
            curl_easy_setopt(handle, CURLOPT_URL, m_url.c_str());
            curl_easy_setopt (handle, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt (handle, CURLOPT_MAXREDIRS, 3L);
            curl_easy_setopt (handle, CURLOPT_AUTOREFERER, 1L);
            curl_easy_setopt (handle, CURLOPT_HEADER, 0L);   //只需要header头
            curl_easy_setopt (handle, CURLOPT_NOBODY, 1L);   //不需要body
            curl_easy_setopt (handle, CURLOPT_FORBID_REUSE, 1);
        	curl_easy_setopt (handle, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36"); //user-agent

            
            //curl_easy_setopt(handle, CURLOPT_HEADER, 1);
            curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
            //curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
            //curl_easy_setopt(handle, CURLOPT_MAXREDIRS, 5);
            //curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
            curl_easy_setopt(handle, CURLOPT_HEADERDATA, &m_receive_header);
            //curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, HttpHelper::RetriveContentFunction);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(handle, CURLOPT_RANGE, "2-");

            CURLcode curl_code = curl_easy_perform(handle);

            if (curl_code == CURLE_OPERATION_TIMEDOUT)
            {
                int retry_count = m_retry_times;
                while (retry_count > 0)
                {
                    curl_code = curl_easy_perform(handle);
                    if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
                    retry_count--;
                }
            }

            curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &m_http_code);

            if (curl_code == CURLE_OK)
            {
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &down_file_length);
            }
            else
            {
               const char* err_string = curl_easy_strerror(curl_code);
               m_error_string = err_string;
            }            

            curl_easy_cleanup(handle);
        }

        return down_file_length;
    }
}

int CHttpDown::SplitDownloadCount(double down_size)
{
    const double size_20mb = 10.0 * 1024 * 1024;
    double startIdx = 0.0;
    int splitDownLoadCount = 0;

    while(down_size > 0)
    {
        double tempDown_size = down_size;
        DownLoadData_s downLoadData;
        downLoadData._startidx = startIdx;
        downLoadData._endidx = (tempDown_size - size_20mb) > 0 ? size_20mb: tempDown_size;
        m_downLoadDataVec.push_back(downLoadData);
        startIdx = downLoadData._endidx;
        down_size = downLoadData._endidx - downLoadData._startidx;
        splitDownLoadCount++;
    }

    return splitDownLoadCount;

}

std::deque<DownLoadData_s>* CHttpDown::getDownLoadData()
{
    return m_downLoadDataDeq;
}


#if 0

int CHttpDown::Perform()
{
    m_total_size = GetDownloadFileSize();
    if (m_total_size < 0)
    {
        return -1;
    }

    std::string out_file_name = m_file_path;
    std::string src_file_name = out_file_name;
    out_file_name += ".dl";

    FILE *fp = nullptr;

    unlink(out_file_name.c_str());
    fp = fopen(out_file_name.c_str(), "wb");

    if (!fp)
    {
        return -1;
    }

    int down_code = -1;
    int split_count = SplitDownloadCount(m_total_size);
    return 0;

}


int CHttpDown::DoDownload()
{
    CURL* curl_handle = curl_easy_init();
    //HttpHelper::set_share_handle(curl_handle);

    if (thread_chunk->_download->m_url.substr(0, 5) == "https")
    {
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, m_url.c_str());

    const char* user_agent = ("Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0");
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 0L);

    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, m_time_out);   //0 means block always

    //curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, HttpHelper::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, thread_chunk);
    //curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, NULL);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
    //curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, HttpHelper::progress_callback);
    //curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, thread_chunk);

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);


    pthread_mutex_lock(&m_mutex);
    DownLoadData_s downLoadData;
    if(!m_taskQueue.empty())
    {
        downLoadData = m_downLoadDataDeq.front();
		m_downLoadDataDeq.pop_front();
    }
    pthread_mutex_unlock(&m_mutex);

    if (downLoadData._endidx != 0)
    {
        std::string down_range;
        std::ostringstream ostr;
        if (thread_chunk->_endidx > 0)
        {
            ostr << downLoadData._startidx << "-" << downLoadData._endidx;
        }
        else
        {
            ostr << downLoadData._startidx << "-";
        }
        
        down_range = ostr.str();
        curl_easy_setopt(curl_handle, CURLOPT_RANGE, down_range.c_str());
    }

    CURLcode curl_code = curl_easy_perform(curl_handle);
    if (curl_code == CURLE_OPERATION_TIMEDOUT)
    {
        int retry_count = m_retry_times;
        while (retry_count > 0)
        {
            curl_code = curl_easy_perform(curl_handle);
            if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
            retry_count--;
        }
    }

    long http_code;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
    if (curl_code == CURLE_OK && (http_code >= 200 && http_code <= 300))
    {
            m_http_code = http_code;
    }
    else
    {
            const char* err_string = curl_easy_strerror(curl_code);
            m_error_string = err_string;
            m_http_code = http_code;
    }

    curl_easy_cleanup(curl_handle);

    return curl_code;
}
#endif

CDownLoadTask::CDownLoadTask()
    : m_downLoadType(0)
    , m_timeOut(0)
    , m_retrytimes(3)
{
}

CDownLoadTask::~CDownLoadTask()
{
    
}

CDownLoadTask::SetDownLoadType(int downLoadType = 0)
{
    m_downLoadType = downLoadType;
}

CDownLoadTask::setDownLoadTaskData(DownLoadData_s *dataLoadData)
{
    m_dataLoadData = dataLoadData;
}

CDownLoadTask::SetRequestUrl(const std::string &url)
{
    m_url = url;
}

CDownLoadTask::SetRetryTimes(int retry_times)
{
    m_retrytimes = retry_times;
}

static size_t CDownLoadTask::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{    
    DownLoadData_s* downLoadData = reinterpret_cast<DownLoadData_s*>(userdata);
    downLoadData->data = (void*)ptr;
    downLoadData->_endidx = downLoadData->_startidx + size * nmemb;
}


int CDownLoadTask::workTaskFun()
{
    CURL* curl_handle = curl_easy_init();
    //HttpHelper::set_share_handle(curl_handle);

    if (m_url.substr(0, 5) == "https")
    {
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYHOST, 0L);
    }

    curl_easy_setopt(curl_handle, CURLOPT_URL, m_url.c_str());

    const char* user_agent = ("Mozilla/5.0 (Windows NT 5.1; rv:5.0) Gecko/20100101 Firefox/5.0");
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 5L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_POST, 0L);

    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, m_time_out);   //0 means block always

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, CDownLoadTask::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, m_dataLoadData);
    //curl_easy_setopt(curl_handle, CURLOPT_HEADERFUNCTION, HttpHelper::RetriveHeaderFunction);
    curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, NULL);

    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
    //curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, HttpHelper::progress_callback);
    //curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, thread_chunk);

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);


    pthread_mutex_lock(&m_mutex);
    DownLoadData_s downLoadData;
    if(!m_taskQueue.empty())
    {
        downLoadData = m_downLoadDataDeq.front();
		m_downLoadDataDeq.pop_front();
    }
    
    if (downLoadData._endidx != 0)
    {
        std::string down_range;
        std::ostringstream ostr;
        if (thread_chunk->_endidx > 0)
        {
            ostr << downLoadData._startidx << "-" << downLoadData._endidx;
        }
        else
        {
            ostr << downLoadData._startidx << "-";
        }
        
        down_range = ostr.str();
        curl_easy_setopt(curl_handle, CURLOPT_RANGE, down_range.c_str());
    }

    CURLcode curl_code = curl_easy_perform(curl_handle);
    if (curl_code == CURLE_OPERATION_TIMEDOUT)
    {
        int retry_count = m_retrytimes;
        while (retry_count > 0)
        {
            curl_code = curl_easy_perform(curl_handle);
            if (curl_code != CURLE_OPERATION_TIMEDOUT) break;
            retry_count--;
        }
    }

    long http_code;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &http_code);
    if (curl_code == CURLE_OK && (http_code >= 200 && http_code <= 300))
    {
            //m_http_code = http_code;
    }
    else
    {
            const char* err_string = curl_easy_strerror(curl_code);
            //m_error_string = err_string;
            //m_http_code = http_code;
    }

    curl_easy_cleanup(curl_handle);

    return curl_code;
}


