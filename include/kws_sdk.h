#ifndef __KWS_SDK_H__
#define __KWS_SDK_H__
#include<stdint.h>
#if defined(__cplusplus)
extern "C" {
#endif
/**
 * @brief: status of kws_engine 
 */
typedef enum class KWS_STATUS{
  kWAKE_UP = 0,
  kUNWAKE_UP,
  kASR
}KWS_STATUS;


typedef enum class KWS_EVENT{

}KWS_EVENT;

typedef enum class DATA_TYPE{

}DATA_TYPE;

typedef enum class CODE_STATUS{
  kCODE_OK = 0,
  kCODE_ERROR
}CODE_STATUS;

typedef struct{
  char* ptr_data;
  uint16_t data_size;
  DATA_TYPE data_type;
}kws_data_t;


/**
 * @brief: sdk config struct.
 */
typedef struct{
  uint16_t input_channels;

  uint32_t sample_rate;

  const char* config_file_path;

  uint16_t data_bit_wide;

  void (*wakeup_callback_func)(const kws_data_t* ptr,void* param);

  void (*event_callback_func)(KWS_EVENT enent_t, void* param);

}kws_config_s;

int kws_init(const kws_config_s* cfg);

int kws_start();

void kws_process(char* raw_ptr,uint32_t raw_size);

int kws_stop();

int kws_destroy();

const char* kws_get_version();

#if defined(__cplusplus)
}
#endif

#endif /*__KWS_SDK_H__*/
