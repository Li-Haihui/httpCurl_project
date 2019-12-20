#include "http_down.h"

#include "thread_pool.h"

CHttpDown::CHttpDown()
{
    m_retry_times = 3;
    m_thread_count = 5;
    m_http_code = 0;
    m_time_out = 0;
    m_mutex = PTHREAD_MUTEX_INITIALIZER;
}

CHttpDown::~CHttpDown()
{

}

int CHttpDown::setRequestUrl(const std::string & url)
{
    m_url = url;
    return 0;
}


int CHttpDown::setTimeout(long time_out)
{
    m_time_out = time_out;
    return 0;
}

int CHttpDown::setDownloadFile(const std::string & file_name)
{
    m_downfile_path = file_name;
    return 0;
}

double CHttpDown::getDownloadFileSize()
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

            curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);

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
                std::cout<<"cur_code:"<<curl_code<<" m_http_code:"<<m_http_code<<std::endl;
                curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &down_file_length);
            }
            else
            {
               const char* err_string = curl_easy_strerror(curl_code);
               std::cout<<"cur_code:"<<curl_code<<" m_http_code:"<<m_http_code<<" err:"<<err_string<<std::endl;

            }            

            curl_easy_cleanup(handle);
        }

        std::cout<<"down_file_length:"<<down_file_length<<std::endl;
        return down_file_length;
    }
}

int CHttpDown::splitDownloadCount(double down_size)
{
    const double size_mb = 10 * 1024;
    double startIdx = 0.0;
    int splitDownLoadCount = 0;
    double tempDown_size = down_size;

    while(tempDown_size > 0)
    {
        DownLoadData_s downLoadData;
        downLoadData._startidx = startIdx;
        double tempWriteSize = (tempDown_size - size_mb) > 0 ? size_mb: tempDown_size;
        downLoadData._endidx = startIdx + tempWriteSize - 1;
        downLoadData.data = (char*)malloc(downLoadData._endidx - downLoadData._startidx + 1);
        m_downLoadDataVec.push_back(downLoadData);
        //std::cout<<downLoadData._startidx<<"-"<<downLoadData._endidx<<std::endl;
        startIdx = downLoadData._endidx + 1;
        tempDown_size = tempDown_size - (downLoadData._endidx - downLoadData._startidx + 1);
        splitDownLoadCount++;
    }

    return splitDownLoadCount;

}

int CHttpDown::doDownLoad()
{
    double down_size = getDownloadFileSize();
    int splitCount = splitDownloadCount(down_size);
    std::cout<<"splitCount:"<<splitCount<<std::endl;
    CDownLoadTask *m_task = new CDownLoadTask[splitCount];
    int threadNum = splitCount > m_thread_count ? m_thread_count : splitCount;
    CThreadPool *threadPool =new CThreadPool(threadNum);
    threadPool->start();
    for(int i = 0; i < splitCount; i++)
    {
        m_task[i].setRequestUrl(m_url);
        m_task[i].setReqId(i);
        m_task[i].setDownLoadType(0);
        m_task[i].setDownLoadTaskData(&m_downLoadDataVec[i]);
        m_task[i].setRetryTimes(m_retry_times);
        m_task[i].setTimeout(m_time_out);
        threadPool->addTask(&m_task[i]);
    }

    delete threadPool;
    FILE *fp;
    fp = fopen(m_downfile_path.c_str(),"wb");
    if(fp == NULL)
    {
        std::cout<<"fopen failed"<<std::endl;
    }
    size_t tolteWriteSize = 0;
    for(int i = 0; i < splitCount; i++)
    {
        size_t written;
        //std::cout<<m_downLoadDataVec[i]._startidx<<"-"<<m_downLoadDataVec[i]._endidx<<std::endl;
        fseek(fp, m_downLoadDataVec[i]._startidx, SEEK_SET);
        std::cout<<m_downLoadDataVec[i]._startidx<<"-"<<m_downLoadDataVec[i]._endidx<<std::endl;
        written = fwrite(m_downLoadDataVec[i].data, 1, m_downLoadDataVec[i]._endidx - m_downLoadDataVec[i]._startidx + 1, fp);
        std::cout<<m_downLoadDataVec[i]._startidx<<"-"<<m_downLoadDataVec[i]._endidx<<" writeSize:"<<written<<std::endl;
        tolteWriteSize += written;
    }

    fclose(fp);
    return 0;
}

