

int main()
{
    const char* down_url = "http://dlsw.baidu.com/sw-search-sp/soft/71/10998/OfflineBaiduPlayer_151_V4.1.2.263.1432003947.exe";
    const char* down_file = "BaiduPlayer.exe";

    CHttpDown httpDown;
    httpDown.SetRequestUrl(down_url);
    
    std::deque<DownLoadData_s> *downLoadData = httpDown.getDownLoadData();
    for(int i = 0; i < downLoadData->size(); i++)
    {
        CDownLoadTask *downLoadTask = new CDownLoadTask();
        
    }
}
