//
// Created by 崔学成 on 2019-03-01.
//

#ifndef SRC_ALSA_HOBOT_SPEECH_RECORD_THREAD_H_
#define SRC_ALSA_HOBOT_SPEECH_RECORD_THREAD_H_
/*
 * Copyright (c) 2019 horizon.ai.
 *
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * horizon.ai, except with written permission of horizon.ai.
 * @author: 崔学成
 * @file: speech_record_thread.h
 * @date: 2019-03-01
 * @brief:
 */

#include <stdint.h>
#include "alsa/asoundlib.h"

#define AUDIO_CHANNEL (3)  // AS05 3通道
#define AUDIO_CHANNEL_BYTES (2)

typedef void (*AudioMicDataCallback)(char *buffer, int size);
typedef void (*AudioRefDataCallback)(char *buffer, int size);

typedef struct {
  snd_pcm_t *capture_handle;
  snd_pcm_hw_params_t *record_params;
  snd_pcm_uframes_t buffer_size;
  snd_pcm_uframes_t period_size;
} AudioRecordParams;

namespace kws_ztx {
class AudioRecordHandler {
 public:
  AudioRecordHandler();
  explicit AudioRecordHandler(int32_t _audio_channel, int32_t _freq);
  virtual ~AudioRecordHandler();

  int RegisterRefCallback(AudioRefDataCallback callback);

  int StartAudioRecord();

  int Init();

  int StopAudioRecoding();

  int DestroyRecordHandler();

 private:
  AudioRecordHandler(const AudioRecordHandler &left);

  const AudioRecordHandler &operator=(const AudioRecordHandler &left);

 private:
  int RecordConfigInit(snd_pcm_t **capture_handle, snd_pcm_hw_params_t **params,
                       AudioRecordParams *record_params, const char *card_name,
                       unsigned int *rate, int channel_num);
  void ReleaseMemory(AudioRecordParams *params);

  static void *RefRecordThread(void *args);

  /**
   * @brief 录音相关配置初始化
   * @return 0 success other failed
   */
  AudioRecordParams *ref_audio_record_params;

  const uint32_t INIT_CONFIG_PERIOD_SIZE = 16 * 16 * 3;

  snd_pcm_format_t ref_pcm_format_;

  unsigned int ref_rate_;

  pthread_t ref_record_thread;

  bool is_inited_;

  static int32_t audio_channel;
};
}  // namespace kws_ztx

#endif  // SRC_ALSA_HOBOT_SPEECH_RECORD_THREAD_H_
