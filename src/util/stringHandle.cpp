// create by tongxiao zhang 
#include "stringHandle.h"
#include <fstream>
#include <iostream>
#include <math.h>
namespace strHandle {
#define CONFIGTAG "HOBOTCONFIG "
void DropCharacter(std::string* _str, char c) {
  std::string& str = *_str;
  str.erase(std::remove(str.begin(), str.end(), c), str.end());
}
std::string IgnorCharacter(const std::string& str, char c) {
  int i = 0;
  for (; i < str.size(); ++i) {
    if (str[i] == c) break;
  }
  if (i == str.size()) return str;
  return str.substr(0, i);
}
std::vector<std::string> splitString(const std::string& _str, char c) {
  int posStart = 0;
  std::string str = IgnorCharacter(_str, '#');
  std::vector<std::string> vStr;
  int i = 0;
  for (; i < str.size(); ++i) {
    if (str[i] == c) {
      vStr.push_back(str.substr(posStart, i - posStart));
      DropCharacter(&vStr.back(), ' ');
      posStart = i + 1;
    }
  }
  if (str.size() >= posStart) {
    vStr.push_back(str.substr(posStart));
    DropCharacter(&vStr.back(), ' ');
  }
  return vStr;
}
char firstCharNotSpace(const std::string& str) {
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] != ' ') return str[i];
  }
  return ' ';
}

int32_t strtous(std::string str, uint16_t* _num) {
  uint16_t num = 0;
  if (str.empty() || (str.size() > 1 && str[0] == '0')) return -1;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] < '0' || str[i] > '9') return -1;
    num = num * 10 + str[i] - '0';
  }
  *_num=num;
  return 0;
}
int32_t strtoi(std::string str, int32_t* _num) {
  int32_t num = 0;
  if (str.empty() || (str.size() > 1 && str[0] == '0')) return -1;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] < '0' || str[i] > '9') return -1;
    num = num * 10 + str[i] - '0';
  }
  *_num=num;
  return 0;
}
int32_t strtou(std::string str, uint32_t* _num) {
  uint32_t num = 0;
  if (str.empty() || (str.size() > 1 && str[0] == '0')) return -1;
  for (int i = 0; i < str.size(); ++i) {
    if (str[i] < '0' || str[i] > '9') return -1;
    num = num * 10 + str[i] - '0';
  }
  *_num=num;
  return 0;
}
/*std::vector<std::string> strtof_split(const std::string& str){
  int pos=-1;
  std::vector<std::string> vstr;
  for(int i=0;i<str.size();++i){
    if(str[i] == '.'){
      if(pos != -1) return;
      pos=i;
    }
  }
}*/
int32_t strtof_point(const std::string& str,float* num){
  int32_t i = 0,tmp=0;
  while (str[i] == '0') ++i;
  std::string strtmp = str.substr(i);
  std::cout<<str<<" --"<<std::endl;
  if (strtmp != "" && strtoi(strtmp, &tmp) != 0) return -1;
  *num = static_cast<float>(tmp) / pow(10, (i + strtmp.size()));
  return 0;
}
int32_t strtof(std::string str, float* _num) {
  float& num = *_num;
  if (str.empty())
    return -1;
  std::vector<std::string> vstr = splitString(str, '.');
  if (vstr.size() > 2) return -1;
  int32_t tmp = 0;
  float ans = 0;
  std::cout<<vstr.size()<<" "<<str[0]<<std::endl;
  if(vstr.size()==1&&str[0]=='.'){
  std::cout<<str[0]<<"+++"<<std::endl;
    return strtof_point(vstr[0], _num);
  }
  if (strtoi(vstr[0], &tmp) != 0) return -1;
  ans += static_cast<float>(tmp);
  if(vstr.size() == 2){
    float tmp_f=0;
    if(strtof_point(vstr[1], &tmp_f) != 0) return -1; 
    ans += tmp_f;
  }
  num = ans;
  return 0;
}
opre_config::opre_config(std::string path) { init(path); }

int opre_config::init(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    return -1;
  } else {
    std::string tmpStr = "";
    while (getline(file, tmpStr)) {
      std::vector<std::string> vStr = strHandle::splitString(tmpStr, '=');
      if (vStr.size() == 2) {
        str_map[vStr[0]] = vStr[1];
      //  std::cout<< vStr[0]<<"="<<vStr[1]<<std::endl;
      }
    }
    file.close();
  }
}

opre_config::~opre_config() {}

std::string opre_config::getvalue(std::string key) {
 // std::cout<<"find key: "<<key<<std::endl;
  auto it = str_map.end();
  if ((it = str_map.find(key)) != str_map.end()) return it->second;
  return "";
}
}  // namespace strHandle
