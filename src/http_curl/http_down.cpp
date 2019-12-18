#include "http_down.h"

CHttpDown::CHttpDown()
    : m_retry_times(3)
    , m_thread_count(5)
    , m_http_code(0)
    , m_time_out(0)
    , m_total_size(0.0)
    , m_mutex(PTHREAD_MUTEX_INITIALIZER)
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
    m_downfile_path = file_name;
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

int CHttpDown::DoDownLoad()
{
    int splitCount = SplitDownloadCount();
    m_task = new CDownLoadTask[splitCount];
    CThreadPool *threadPool =new CThreadPool(5);
    for(int i = 0; i < splitCount; i++)
    {
        m_task[i].SetRequestUrl(m_url);
        m_task[i].SetDownLoadType(0);
        m_task[i].setDownLoadTaskData(&m_downLoadDataDeq[i]);
        m_task[i].SetRetryTimes(m_retry_times);
        m_task[i].SetTimeout(m_time_out);
        threadPool.addTask(m_task[i]);
    }

    delete threadPool;
    FILE *fp;
    fp = fopen(m_downfile_path.c_str(),"wb");
    size_t tolteWriteSize = 0; 
    for(int i = 0; i < splitCount; i++)
    {
        fseek(fp, m_downLoadDataDeq[i]._startidx, SEEK_SET);
        written = fwrite(m_downLoadDataDeq[i].data, 1, m_downLoadDataDeq[i]._endidx - m_downLoadDataDeq[i]._startidx + 1, fp);
        tolteWriteSize += written;
    }

    fclose(fp);

    
}

