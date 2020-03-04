#include "kwslog/log.hpp"
namespace rtc{
std::ostringstream massage::sstr;
rtc::LOG_LEVEL massage::level=rtc::LOG_LEVEL::ERROR;
}
void SetLogLevel(rtc::LOG_LEVEL _lev){
  rtc::massage::level=_lev;
}
