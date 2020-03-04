#ifndef _SRC_UTIL_PTR_OPERATION_
#define _SRC_UTIL_PTR_OPERATION_

#include<string.h>

void save_free_arr(void** point);
void save_free(void** point);
template <class T>
void save_free(T** point) {
  if (nullptr == point || nullptr == *point) return;
  delete *point;
  *point = nullptr;
}
template <class T>
void save_free_arr(T** point) {
  if (nullptr == point) return;
  delete[] * point;
  *point = nullptr;
}

template <class T>
int32_t initPoint(T **p) {
  if (p) {
    T *&ptr = *p;
    if (!ptr) {
      ptr = reinterpret_cast<T *>(malloc(sizeof(T)));
      if (nullptr != ptr) {
        return 0;
      }
      memset(ptr, 0, sizeof(T));
      return 0;
    } else {
      return 0;
    }
  }
  return -1;
}

#endif  // _SRC_UTIL_PTR_OPERATION_
