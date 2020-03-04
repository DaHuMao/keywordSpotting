/*****************************************************************************
 * Created by tongxiao.zhang
 * Date : 2019/12/17
 *****************************************************************************/
#include <sound/asound.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "src/record/tinyalsa_record.h"

namespace kws_ztx {

AudioRecordTiny AudioRecordTiny::audio_handle;

AudioRecordTiny::AudioRecordTiny() {
  pcm = nullptr;
  buffer = nullptr;
  record_callback = nullptr;
}

AudioRecordTiny::~AudioRecordTiny() {
  delete[] buffer;
  buffer = nullptr;
}

int AudioRecordTiny::record_init(size_t rate, size_t channels,
                                 record_Callback callback_fun) {
  if (pcm != nullptr) {
    return 0;
  }
  _rate = rate;
  _channels = channels;
  record_callback = callback_fun;
  int ret = 0;

  fprintf(stderr, "channel: %d , rate: %d \n", _channels, _rate);
  memset(&config, 0, sizeof(config));
  config.channels = _channels;
  config.rate = _rate;
  config.format = PCM_FORMAT_S16_LE;
  config.period_size = 1024;
  config.period_count = 4;
  config.start_threshold = 0;
  config.stop_threshold = 0;
  config.silence_threshold = 0;
  // audio init
  pcm = pcm_open(0, 0, PCM_IN, &config, 0);
  if (pcm == NULL) {
    fprintf(stderr, "failed to allocate memory for PCM\n");
    return -1;
  } else if (!pcm_is_ready(pcm)) {
    pcm_close(pcm);
    fprintf(stderr, "failed to open PCM\n");
    return -1;
  }
  if (nullptr == pcm || !pcm_is_ready(pcm)) {
    fprintf(stderr, "open error:%s\n", pcm_get_error(pcm));
    return -1;
  }
  _size = pcm_frames_to_bytes(pcm, pcm_get_buffer_size(pcm));
  buffer = new char[_size];
  // thread init
  ret = pthread_create(&audio_tid, nullptr, record_thread, this);
  if (0 != ret) {
    pcm_close(pcm);
    perror("create pthread failed");
    return -1;
  }

  return 0;
}

int AudioRecordTiny::record_destroy() { pcm_close(pcm); }
void *AudioRecordTiny::record_thread(void *arg) {
  pthread_detach(pthread_self());
  AudioRecordTiny *pThis = static_cast<AudioRecordTiny *>(arg);
  pThis->record_process();
  return nullptr;
}
int AudioRecordTiny::record_process() {
  int frames_read = 0;
  while (1) {
    frames_read = pcm_readi(pcm, buffer, pcm_get_buffer_size(pcm));
    if (frames_read != config.period_size * config.period_count) {
      usleep(4 * 16 * 1000);
      continue;
    }
    if (nullptr != record_callback) {
      record_callback(buffer, frames_read * 4);
    } else {
      usleep(10 * 1000);
      fprintf(stderr, "callback is nullptr\n");
    }
  }

  return 0;
}

}  // kws_ztx
