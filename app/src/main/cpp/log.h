//
// Created by dong on 2018/11/5.
//

#ifndef LOG_H
#define LOG_H

#include <android/log.h>
#include <string>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void LogI(std::string tag, std::string msg) {
    __android_log_print(ANDROID_LOG_INFO,tag.c_str(), msg.c_str());
}

void LogE(std::string tag, std::string msg) {
    __android_log_print(ANDROID_LOG_ERROR,tag.c_str(), msg.c_str());
}

#endif //LOG_H
