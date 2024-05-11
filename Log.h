#ifndef _LOG_H
#define _LOG_H
#include<iostream>
#include<string>
#include<time.h>
#include<vector>
#pragma warning( disable : 4996 )
using namespace std;
inline std::string getTime() {
    const char* time_form = "%Y-%m-%d %H:%M:%S";
    time_t t = time(nullptr);//获取当前时间
    char now_time[64];
    strftime(now_time, sizeof(now_time), time_form, localtime(&t));
    return now_time;
}
static std::string getFile(string file) {
    string pattern = "\\";
    string::size_type pos;
    std::vector<std::string> result;
    file += pattern;//扩展字符串以方便操作
    int size = file.size();
    for (int i = 0; i < size; i++) {
        pos = file.find(pattern, i);//查找指定字符串
        if (pos < size) {
            std::string s = file.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result.back();//返回容器最后面的元素
}
#define LOGI(format, ...)  fprintf(stderr,"[INFO]%s [%s:%d] " format "\n", getTime().data(),__FILE__,__LINE__,##__VA_ARGS__)
#define LOGE(format, ...)  fprintf(stderr,"[ERROR]%s [%s:%d] " format "\n",getTime().data(),__FILE__,__LINE__,##__VA_ARGS__)
#endif