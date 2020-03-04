// Copyright horizon 2019.11.26
#include <inttypes.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "example/audiofile.h"
#include "example/file_utils.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif
#include "./kws_sdk.h"
char* save_data_path;

#define LOG_TAG "MiVpmDemo"

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define AUDIO_FILE_PATH "/data/kws/data"

int count_data = 0;
int count_data_i = 0;
uint64_t file_length(FILE* f) {
  uint64_t len;

  fseek(f, 0, SEEK_END);
  len = ftell(f);
  fseek(f, 0, SEEK_SET);
  return len;
}

char* file_read_buf(char* fn, int* n) {
  FILE* file = fopen(fn, "rb");
  char* p = 0;
  int len;

  if (file) {
    len = file_length(file);
    p = reinterpret_cast<char*>(malloc(len + 1));
    len = fread(p, 1, len, file);
    if (n) {
      *n = len;
    }
    fclose(file);
    p[len] = 0;
  }
  return p;
}



FILE* file;
int main(int argc, char* argv[]) {
  kws_config_s kws_cfg;
  printf("zhang tong xiao is test\n");
  if (argc < 2) {
    printf(
        "Usage: [-i][-o][-cfg]\n\n-i: input file absoulute path\n -o: outfile "
        "folder\n -cfg:cfg path kws folder path\n");
    exit(0);
  }
  char *fn, *data, *cfg_path;
  int mode = 0;
  while (*argv) {
    if (strcmp(*argv, "-i") == 0) {
      argv++;
      fn = *argv;
      printf("source file path is %s\n", fn);
    } else if (strcmp(*argv, "-o") == 0) {
      argv++;
      save_data_path = *argv;
      printf("save file folder is %s\n", save_data_path);
    } else if (strcmp(*argv, "-cfg") == 0) {
      argv++;
      cfg_path = *argv;
      printf("cfg file path is %s\n", cfg_path);
    } else if (strcmp(*argv, "-m") == 0) {
      argv++;
      mode = atoi(*argv);
      printf("mode is %d\n", mode);
    }
    if (*argv) {
      argv++;
    }
  }

  int len;
  int step, pos, n;

  kws_cfg.input_channels = 6;


  kws_cfg.config_file_path = cfg_path;

  kws_cfg.sample_rate = 48000;

  kws_cfg.data_bit_wide = 2;

  printf("kws_sdk version: %s\n",kws_get_version());
  printf("start init .... \n");
  if (0 != kws_init((const kws_config_s*)&kws_cfg)) {
    exit(1);
  }

  printf("end init .... \n");
  if (0 != kws_start()) {
    printf("start error\n");
    kws_destroy();
    exit(1);
  }
#ifdef ALSA_RECORD_MODE
  while (1) {
    sleep(1000);
  }
#endif
#ifdef TINYALSA_RECORD_MODE
  while (1) {
    sleep(1000);
  }
#endif
  pos = 0;
  step = 32 * 16 * kws_cfg.input_channels;
  char* audio_buffer = new char[step];
  int read_size = 0;
  int numCir = 0;
  while (numCir < 1) {
    file = fopen(fn, "rb");
    if (!file) {
      printf("data read fail\n");
      exit(1);
    }
    if (strstr(fn, ".wav") != nullptr) {
      hdr_t hdr;
      int wav_len = GetWaveHeadLen(&hdr, file);
      fseek(file, 0, SEEK_SET);
      fseek(file, wav_len, SEEK_SET);
    }
    while ((read_size = fread(audio_buffer, sizeof(char), step, file)) > 0) {
      kws_process(audio_buffer,read_size);
      usleep(16  * 1000 );
    }
    numCir++;
    fclose(file);
  }
  delete[] audio_buffer;

  /*
   * 阻塞接口，当所有数据处理完成后kws_stop才会返回
   */
  kws_stop();
  kws_destroy();
  return 0;
}
