#ifndef _HTTP_CURL_H
#define _HTTP_CURL_H

#include <string>


class CHttpCurl
{
    public:
        CHttpCurl();
        virtual ~CHttpCurl();
    private:
        std::string					url_;
};

#endif
