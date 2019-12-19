#include "http_down.h"

int main()
{
    //const char* down_url = "http://dlsw.baidu.com/sw-search-sp/soft/71/10998/OfflineBaiduPlayer_151_V4.1.2.263.1432003947.exe";
    const char* down_url = "http://cdimage.ubuntu.com/releases/16.04/release/ubuntu-16.04.4-preinstalled-server-armhf+raspi2.manifest";
    const char* down_file = "BaiduPlayer.exe";

    CHttpDown httpDown;
    httpDown.setRequestUrl(down_url);
    httpDown.doDownLoad();
    
}
