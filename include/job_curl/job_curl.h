#ifndef _JOB_CURL_H
#define _JOB_CURL_H

#include "curl.h"

template<typename T>
class Curl
{
    public:
        Curl();
        virtual ~Curl();
        bool init(void* rspData);
    	bool setOption(CURLoption option, T data);
        bool perform(long* responseCode);
    private:
	    CURL*		m_curl;
};


class CJobCurl:public CBaseTask
{
    public:
        CJobCurl(int startPos, int endPos, std::string downFilePath, std::string downUrl);
        virtual ~CJobCurl();
        virtual int workTaskFun();
        void setDownFp(FILE* downFp);
        void setRsp(void *rspData);
        Curl* getCcurl();
        long getStartPos();
        long getEndPos();
        FILE* getDownFp();
    private:
        static size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata); 
    private:
        long m_startPos;
        long m_endPos;
        FILE* m_downFp;
        //std::string m_downUrl;
        Curl m_Ccurl;
        
        //void* m_rspData;
    
};

#endif
