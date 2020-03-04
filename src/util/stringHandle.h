// create by tongxiao.zhang
#ifndef SRC_UTIL_STRINGHANDLE_H_
#define SRC_UTIL_STRINGHANDLE_H_
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>
namespace strHandle {
void DropCharacter(std::string* str, char c);
std::string IgnorCharacter(const std::string& str, char c);
std::vector<std::string> splitString(const std::string& _str, char c);
char firstCharNotSpace(const std::string& str);
int32_t strtos(std::string str, uint16_t* num);
int32_t strtoi(std::string str, int32_t* num);
int32_t strtof(std::string _str, float* _num);
class opre_config {
 public:
  explicit opre_config(std::string path);
  ~opre_config();
  std::string getvalue(std::string key);

 private:
  int init(const std::string& path);
  std::unordered_map<std::string, std::string> str_map;
};
}  // namespace strHandle
#endif  // SRC_UTIL_STRINGHANDLE_H_
