#include "job_curl.h"


//每个线程在下载每个文件分片时,都会回调该函数
static size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    size_t written = 0;
    pthread_mutex_lock (&g_mutex);
    if(m_startPos + size * nmemb <= ji->stopPos){
        fseek(ji->fp, ji->startPos, SEEK_SET);
        written = fwrite(ptr, size, nmemb, ji->fp);
        ji->startPos += size * nmemb;
    }
    else{
        fseek(ji->fp, ji->startPos, SEEK_SET);
        written = fwrite(ptr, 1, ji->stopPos - ji->startPos + 1, ji->fp);
        ji->startPos = ji->stopPos;
    }
    pthread_mutex_unlock (&g_mutex);

    return written;
}



Curl::Curl() 
{
    m_curl = curl_easy_init();
}

Curl::~Curl()
{
	if (m_curl)
    {
        curl_easy_cleanup(m_curl);
    }
}

bool Curl::init(void* rspData)
{
	if (m_curl == nullptr) return false;
	/*set no signal*/
	if (!setOption(CURLOPT_NOSIGNAL, 1L)) return false;

	/*set accept encoding*/
	if (!setOption(CURLOPT_ACCEPT_ENCODING, "")) return false;
    
    return setOption(CURLOPT_WRITEFUNCTION, writeCallback) &&
           setOption(CURLOPT_WRITEDATA, rspData) &&
           setOption(CURLOPT_RANGE, 1L);
}

bool Curl::setOption(CURLoption option, T data)
{
	return CURLE_OK == curl_easy_setopt(m_curl, option, data);
}

bool Curl::perform(long* responseCode)
{
	if (CURLE_OK != curl_easy_perform(m_curl)) return false;

	CURLcode code = curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, responseCode);
	if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300))
	{
		std::cout<<"curl_easy_perform() failed:"<<curl_easy_strerror(code)<<std::endl;
		return false;
	}

	return true;
}



CJobCurl::CJobCurl(int startPos, int endPos, std::string downFilePath, std::string downUrl):
    m_startPos(startPos),m_endPos(endPos),m_downFilePath(downFilePath),m_downUrl(downUrl)
{
    m_Ccurl.init(this);
    
}

CJobCurl::~CJobCurl()
{
}

CJobCurl::setDownFilePath(FILE* downFp)
{
    m_downFp = downfp;
}

int CJobCurl::workTaskFun()
{
    long responseCode = 0;
    return m_Ccurl.perform(&responseCode);
}

Curl* CJobCurl::getCcurl()
{
    return &m_Ccurl;
}

void CJobCurl::setRsp(void * rspData)
{
    m_rspData = rspData;
}



