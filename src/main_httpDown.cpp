#include "http_down.h"

int main()
{
    //const char* down_url = "http://cdimage.ubuntu.com/releases/16.04/release/ubuntu-16.04.4-preinstalled-server-armhf+raspi2.manifest";
    //const char* down_file = "ubuntu-16.04.4-preinstalled-server-armhf+raspi2.manifest";

    //const char* down_url = "http://www.baidu.com/img/baidu.gif";
    //const char* down_file = "baidu.gif";

    const char* down_url = "http://e.hiphotos.baidu.com/image/pic/item/dc54564e9258d1092f7663c9db58ccbf6c814d30.jpg";
    const char* down_file = "down_1.jpg";

    CHttpDown httpDown;
    httpDown.setRequestUrl(down_url);
    httpDown.setDownloadFile(down_file);
    httpDown.doDownLoad();
    
}
