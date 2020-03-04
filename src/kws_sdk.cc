#include "kws_sdk.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <string>
#include "engine/kws_factory.h"
#include "engine/kws_engine.h"
#include "circ_buff.h"
//#include "kwslog/kwslog.hpp"
#include "kwslog/log.hpp"
#ifdef ALSA_RECORD_MODE
#include "record/speech_record_thread.h"
#endif
#define LOG_TAG "KWS_SDK"
unsigned long g_kws_audio_size = 0;

const kws_config_s *g_cfg;

int is_start = 0;

// Data length of one frame and one channel
static const int single_bufferSize = 16 * 16 * 2;

static const int cir_buff_frame = 180;

tCircularBuffer *raw_buffer = nullptr;

#ifdef ALSA_RECORD_MODE
FILE *fin = nullptr;
int16_t *record_buff = nullptr;
const int32_t src_num = 1, des_num = 1, freq_num = 48000;
static kws_ztx::AudioRecordHandler audio_record_handler(src_num,
                                                              freq_num);
int32_t Supplement_zero(int16_t *src_buf, int32_t src_size, int16_t *des_buf,
                        int32_t des_size, int32_t src_chan, int32_t des_chan) {
  if (des_size * src_chan < src_size * des_chan || des_chan < src_chan)
    return -1;
  for (int i = 0; i < src_size / src_chan; ++i) {
    for (int j = 0; j < des_chan; ++j) {
      if (j < src_chan)
        des_buf[i * des_chan + j] = src_buf[src_chan * i + j];
      else
        des_buf[i * des_chan + j] = 0;
    }
  }
  return 0;
}
void AudioRefCallback(char *audio_buff, int size) {
  int des_size = size * des_num / src_num / sizeof(int16_t);
  if (nullptr == record_buff) {
    if (des_num == src_num) {
      record_buff = reinterpret_cast<int16_t *>(audio_buff);
    } else {
      record_buff = new int16_t[des_size * 2];
    }
  }
  if (des_num > src_num)
    Supplement_zero(reinterpret_cast<int16_t *>(audio_buff), size / 2,
                    record_buff, des_size, src_num, des_num);
  if (fin) fwrite(record_buff, sizeof(int16_t), des_size, fin);
  FillBuffer(raw_buffer, reinterpret_cast<char *>(record_buff),
             des_size * sizeof(int16_t));
}
#endif


uint32_t input_audio_data(char *audiodata, uint32_t audio_len) {
  int32_t readlen = 0;
  while (1 && is_start) {
    readlen = ReadBuffer(raw_buffer, audiodata, audio_len);
    if (readlen <= 0) {
      usleep(10 * 1000);
      continue;
    } else {
      break;
    }
  }
  return readlen;
}





void m_getLogLevel(const std::string &level) {
  if (level == "INFO")  SetLogLevel(rtc::LOG_LEVEL::INFO);
  if (level == "DEBUG") SetLogLevel(rtc::LOG_LEVEL::DEBUG);
  if (level == "WARN")  SetLogLevel(rtc::LOG_LEVEL::WARN);
  if (level == "ERROR")  SetLogLevel(rtc::LOG_LEVEL::ERROR);
}


static std::ofstream *fkws_log = nullptr;
static std::streambuf *backup = nullptr;
CODE_STATUS kws_Init(kws_config_s *config) {
  std::string kws_cfg_path =
      (std::string)config->config_file_path + "/kws_config.ini";
  strHandle::opre_config opc(kws_cfg_path);
  std::string log_level = opc.getvalue("SDK_LOG_LEVEL");
  m_getLogLevel(log_level);
  std::string log_flag = opc.getvalue("SAVE_LOG_FLAG");
  if ("1" == log_flag) {
    std::string save_log_path = opc.getvalue("SAVE_LOG_PATH");
    if (!save_log_path.empty()) {
      save_log_path += "/kws_log.txt";
      FILE *flog = freopen(save_log_path.c_str(), "w", stderr);
      if (nullptr == flog) {
        LOGE_T(KWSAG) << "Failed to redirect log to file: " << save_log_path;
      }
    }
  }
  if (kws_ztx::factory::getInstance().product(config) != CODE_STATUS::kCODE_OK) {
    LOGE_T(KWSAG) << "failed producted";
    return CODE_STATUS::kCODE_ERROR;
  }
  return kws_ztx::KwsEngine::instance().init();
}


int kws_init(const kws_config_s *config) {
  int ret = 0;
  SetLogLevel(rtc::LOG_LEVEL::INFO);
  if (NULL == config) {
    return -1;
  }

  g_cfg = config;

// init kws and create object
#ifdef ALSA_RECORD_MODE
  audio_record_handler.RegisterRefCallback(AudioRefCallback);
  ret = audio_record_handler.Init();
  if (ret != 0) {
    LOGI_T(LOG_TAG) << "audio_record_handler init error";
    return -1;
  }
#endif
  if (kws_Init((kws_config_s*)g_cfg) != CODE_STATUS::kCODE_OK) {
    LOGE_T(LOG_TAG) << "kws init error: ";
    return -1;
  }
  kws_ztx::KwsEngine::instance().set_engine_param( \
                reinterpret_cast<void*>(input_audio_data), \
                kws_ztx::KWS_ENGINE_PARAM::kINPUT_DATA_CB);
  if (!raw_buffer) {
    // Size of loop BUFF
    raw_buffer = CreateCircularBuffer(single_bufferSize * cir_buff_frame *
                                      g_cfg->input_channels);
  }
  return 0;
}

int kws_start() {
  LOGI_T(LOG_TAG)<<"start kws_engine";
  int ret = 0;
#ifdef ALSA_RECORD_MODE
  ret = audio_record_handler.StartAudioRecord();
  if (0 != ret) {
    LOGE_T(LOG_TAG) << "audio_record_handler start error";
    return -1;
  }
#endif
  is_start = 1;
  ret = kws_ztx::KwsEngine::instance().startEngine();
  if (ret) {
    LOGE_T(LOG_TAG)<<"failed start kws_engine";
    return -1;
  }
  return 0;
}

int kws_stop() {
  is_start=0;
  kws_ztx::KwsEngine::instance().stop();
  return 0;
}

int kws_destroy() {
#ifdef ALSA_RECORD_MODE
  if (nullptr != record_buff) {
    delete record_buff;
    record_buff = nullptr;
  }
  audio_record_handler.StopAudioRecoding();
#endif
  if(nullptr != raw_buffer){
    DestroyCircularBuffer(raw_buffer);
    raw_buffer = nullptr;
  }
  kws_ztx::KwsEngine::instance().destoryEngine();
  return 0;
}



void kws_process(char* raw_ptr,uint32_t raw_size) {
  if (nullptr == raw_ptr) {
    return;
  }

  if (IsBufferSizeFilled(raw_buffer, 0)) {
    LOGW_T(LOG_TAG) << "The buffer is full and half the data will be discarded";
    int bufferSize =
        single_bufferSize * cir_buff_frame * g_cfg->input_channels;
    char temp[bufferSize / 2];
    ReadBuffer(raw_buffer, temp, (bufferSize / 2));
  }
  int32_t ret = static_cast<int32_t>(
      FillBuffer(raw_buffer, reinterpret_cast<char *>(raw_ptr), raw_size*sizeof(char)));
  if (ret == -1) {
    LOGW_T(LOG_TAG) << "process data too quickly,please slow down";
    return;
  }
  return;
}

const char* kws_get_version(){
  return "0.0.1";
}
