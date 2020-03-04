#ifndef KWSLOG_KWSLOG_HPP
#define KWSLOG_KWSLOG_HPP

#include <iostream>
#include <sstream>
#include <stdio.h>

namespace rtc{
typedef enum class LOG_LEVEL{
  DEBUG = 0,
  INFO,
  WARN,
  ERROR
}LOG_LEVEL;
class massage{
public:
    massage(const char* tag,LOG_LEVEL lev):m_lev(lev){
        if(m_lev >= level)
            fprintf(stderr,"%s: ",tag); 
    }
    ~massage(){
        if(m_lev >= level)
            fprintf(stderr,"\n"); 
    }
    static LOG_LEVEL level;
    template<class T>
      massage& operator<< (T&& val){
        if(m_lev >= level){
          sstr.str("");
          sstr<<val;
          fprintf(stderr,"%s",sstr.str().c_str());
        }
        return *this;
      }
private:
    LOG_LEVEL m_lev;
    static std::ostringstream sstr;
};
}  //rtc
void SetLogLevel(rtc::LOG_LEVEL _lev);
#define LOGE_T(tag) rtc::massage(tag,rtc::LOG_LEVEL::ERROR)
#define LOGW_T(tag) rtc::massage(tag,rtc::LOG_LEVEL::WARN)
#define LOGI_T(tag) rtc::massage(tag,rtc::LOG_LEVEL::INFO)
#define LOGD_T(tag) rtc::massage(tag,rtc::LOG_LEVEL::DEBUG)

#endif //KWSLOG_KWSLOG_HPP
