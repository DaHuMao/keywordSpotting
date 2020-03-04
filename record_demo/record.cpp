// Copyright horizon 2019.11.26
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "record/speech_record_thread.h"
#include "util/stringHandle.h"
FILE* fin = nullptr;
int32_t Supplement_zero(int16_t* src_buf, int32_t src_size, int16_t* des_buf,
                        int32_t des_size, int32_t src_chan, int32_t des_chan) {
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
int16_t* buf = nullptr;
int32_t src_num = 0, des_num = 0, freq_num = 0;
void AudioRefCallback(char* audio_buff, int size) {
  int des_size = size * des_num / src_num / sizeof(int16_t);
  if (nullptr == buf) {
    if (des_num == src_num) {
      buf = reinterpret_cast<int16_t*>(audio_buff);
    } else {
      buf = new int16_t[des_size];
    }
  }
  Supplement_zero(reinterpret_cast<int16_t*>(audio_buff), size / 2, buf,
                    des_size, src_num, des_num);
  if (fin) fwrite(buf, sizeof(int16_t), des_size, fin);
}

int main(int argc, char* argv[]) {
  const char* w_str = "./record 2 4 1600 that means src:2 des:4 rate:1600";
  if (argc < 4) {
    printf("you should input two param: ex: %s\n", w_str);
    exit(0);
  }
#ifdef ALSA_RECORD_MODE
  printf("=================^start record^====================\n");
#endif
  std::string mic_str(argv[1]);
  std::string ref_str(argv[2]);
  std::string freq_str(argv[3]);
  fin = fopen("./record.pcm", "wb+");
  if (!fin) {
    printf("faild save record");
    exit(0);
  }
  if (strHandle::strtoi(mic_str, &src_num) != 0) {
    printf("param %s error", argv[1]);
    exit(0);
  }

  if (strHandle::strtoi(ref_str, &des_num) != 0) {
    printf("param %s error", argv[2]);
    exit(0);
  }
  if (strHandle::strtoi(freq_str, &freq_num) != 0) {
    printf("param %s error", argv[3]);
    exit(0);
  }
  kws_ztx::AudioRecordHandler audio_record_handler(src_num, freq_num);
  audio_record_handler.RegisterRefCallback(AudioRefCallback);
  int ret = audio_record_handler.Init();
  if (ret != 0) {
    printf("audio_record init error: %d\n", ret);
    exit(0);
  }
  ret = audio_record_handler.StartAudioRecord();
  if (ret != 0) {
    printf("audio_record start error: %d\n", ret);
    exit(0);
  }
  sleep(3600);
  if (nullptr != buf) {
    delete buf;
    buf = nullptr;
  }
  // audio_record_handler.StopAudioRecoding();
}
