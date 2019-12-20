#include "down_loadTask.h"

#include<cstring>


CDownLoadTask::CDownLoadTask()
    : CBaseTask(0)
{
    m_reqId = 0;
}

CDownLoadTask::~CDownLoadTask()
{
    
}

void CDownLoadTask::setReqId(int reqId)
{
    m_reqId= reqId;
    CBaseTask(m_reqId);
}


void CDownLoadTask::setDownLoadType(int downLoadType)
{
    m_downLoadType = downLoadType;
}

void CDownLoadTask::setDownLoadTaskData(DownLoadData_s *downLoadData)
{
    m_downLoadData = downLoadData;
}

void CDownLoadTask::setTimeout(long time_Out)
{
    m_timeOut = time_Out;
}


void CDownLoadTask::setRequestUrl(const std::string &url)
{
    m_url = url;
}

void CDownLoadTask::setRetryTimes(int retry_times)
{
    m_retrytimes = retry_times;
}

size_t CDownLoadTask::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{    
    DownLoadData_s* downLoadData = reinterpret_cast<DownLoadData_s*>(userdata);
    std::cout<<"begin:"<<downLoadData->_curridx<<" size:"<<size * nmemb<<std::endl;
    memcpy((uint8_t *)downLoadData->data + downLoadData->_curridx, ptr, size * nmemb);
    downLoadData->_curridx = downLoadData->_curridx + size * nmemb;
    return size * nmemb;
}


int CDownLoadTask::workTaskFun()
{
    std::cout<<"begin"<<std::endl;
    CURL* curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, m_url.c_str());

    const char* user_agent = ("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/36.0.1985.143 Safari/537.36");
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

    curl_easy_setopt(curl_handle, CURLOPT_MAXREDIRS, 3L);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 5L);

    curl_easy_setopt(curl_handle, CURLOPT_CONNECTTIMEOUT_MS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, m_timeOut);   //0 means block always

    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, CDownLoadTask::write_callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, m_downLoadData);



    if (m_downLoadData->_endidx != 0)
    {
        std::string down_range;
        std::ostringstream ostr;
        if (m_downLoadData->_endidx > 0)
        {
            ostr << m_downLoadData->_startidx << "-" << m_downLoadData->_endidx;
        }
        else
        {
            ostr << m_downLoadData->_startidx << "-";
        }
        
        down_range = ostr.str();
        std::cout<<"m_reqId:"<<m_reqId<<"%%%%"<<"down_range:"<<down_range<<std::endl;
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
        std::cout<<"cur_code:"<<curl_code<<" http_code:"<<http_code<<std::endl;
    }
    else
    {
        const char* err_string = curl_easy_strerror(curl_code);
        std::cout<<"cur_code:"<<curl_code<<" http_code:"<<http_code<<" err:"<<err_string<<std::endl;
            
    }

    curl_easy_cleanup(curl_handle);

    return curl_code;
}


