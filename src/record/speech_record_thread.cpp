/*
 * Copyright (c) 2019 horizon.ai.
 *
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * horizon.ai, except with written permission of horizon.ai.
 * @author: 崔学成
 * @file: speech_record_thread.cpp
 * @date: 2019-03-01
 * @brief: alsa录音
 */
#include "record/speech_record_thread.h"
#include <sys/prctl.h>
#include "kwslog/log.hpp"

static const char *kAudioRecordTag = "AudioRecord";

// 回调函数
static AudioMicDataCallback audio_mic_callback = nullptr;
static AudioRefDataCallback audio_ref_callback = nullptr;

// 线程处理标志位
static volatile bool audio_start_flag = false;

namespace kws_ztx {

#if 0
static void *MicRecordThread(void *args) {
  prctl(PR_SET_NAME,  "MicThread");

  LOGI_T(kAudioRecordTag) << "mic record thread start";

  AudioRecordParams *params = static_cast<AudioRecordParams *>(args);
  int32_t err = -1;

  int size = params->period_size * MIC_CHANNEL_BYTES * MIC_CHANNEL;

  LOGI_T(kAudioRecordTag) << "period size is" << params->period_size
                          << ", mic record size is " << size;

  char *audio_buffer = reinterpret_cast<char*>(malloc(size));

  while (audio_start_flag) {
    err = snd_pcm_readi(params->capture_handle, audio_buffer,
                        params->period_size);

    if (err == EPIPE) {
      fprintf(stderr, "take mic data , err is %d\n", err);
      snd_pcm_prepare(params->capture_handle);
      usleep(2 * 1000);
      continue;
    }

    if (err == EAGAIN || err < 0) {
      LOGE_T(kAudioRecordTag) << "record EAGAIN err is " << err;
      continue;
    }
    audio_mic_callback(audio_buffer, err * MIC_CHANNEL * MIC_CHANNEL_BYTES);
  }

  free(audio_buffer);

  return nullptr;
}
#endif

void *AudioRecordHandler::RefRecordThread(void *args) {
  prctl(PR_SET_NAME, "RefThread");
  LOGI_T(kAudioRecordTag) << "ref record thread start";
  AudioRecordParams *params = static_cast<AudioRecordParams *>(args);
  int32_t err = -1;

  int size = params->period_size * AUDIO_CHANNEL_BYTES * audio_channel;
  LOGI_T(kAudioRecordTag) << "period size is" << params->period_size
                          << ", ref record size is " << size;

  char *audio_buffer = reinterpret_cast<char *>(malloc(size));
  if (audio_buffer == nullptr) {
    LOGE_T(kAudioRecordTag) << "malloc " << size << " buffer failed";
    return nullptr;
  }

  while (audio_start_flag) {
    err = snd_pcm_readi(params->capture_handle, audio_buffer,
                        params->period_size);

    if (err == -EPIPE) {
      fprintf(stderr, "take ref data , err is %d\n", err);
      snd_pcm_prepare(params->capture_handle);
      continue;
    }

    if (err < 0) {
      LOGE_T(kAudioRecordTag) << "record EAGAIN err is " << err;
      snd_pcm_prepare(params->capture_handle);
      continue;
    }

    audio_ref_callback(audio_buffer, err * audio_channel * AUDIO_CHANNEL_BYTES);
  }

  free(audio_buffer);

  return nullptr;
}

int32_t AudioRecordHandler::audio_channel = AUDIO_CHANNEL;

AudioRecordHandler::AudioRecordHandler()
    : ref_audio_record_params(nullptr),
      ref_pcm_format_(SND_PCM_FORMAT_S16_LE),
      ref_rate_(16000),
      ref_record_thread(),
      is_inited_(false) {
  ref_audio_record_params = new AudioRecordParams();
}

AudioRecordHandler::AudioRecordHandler(int32_t _audio_channel, int32_t _freq)
    : ref_audio_record_params(nullptr),
      ref_pcm_format_(SND_PCM_FORMAT_S16_LE),
      ref_rate_(_freq),
      ref_record_thread(),
      is_inited_(false) {
  audio_channel = _audio_channel;
  ref_audio_record_params = new AudioRecordParams();
}

AudioRecordHandler::~AudioRecordHandler() {
  LOGE_T(kAudioRecordTag) << "start release all resources";

  LOGI_T(kAudioRecordTag) << "end release resources";
}

int AudioRecordHandler::Init() {
  int ret;  // 单mic

  ret = RecordConfigInit(&ref_audio_record_params->capture_handle,
                         &ref_audio_record_params->record_params,
                         ref_audio_record_params, "hw:0,0", &ref_rate_,
                         audio_channel);
  if (ret != 0) {
    LOGE_T(kAudioRecordTag) << "ref audio record init failed ";
    return -1;
  }

  LOGI_T(kAudioRecordTag) << "init finished";

  return 0;
}

int AudioRecordHandler::StartAudioRecord() {
  audio_start_flag = true;

  is_inited_ = true;

  pthread_create(&ref_record_thread, nullptr, RefRecordThread,
                 ref_audio_record_params);

  return 0;
}

int AudioRecordHandler::RecordConfigInit(snd_pcm_t **capture_handle,
                                         snd_pcm_hw_params_t **params,
                                         AudioRecordParams *record_params,
                                         const char *card_name,
                                         unsigned int *rate, int channel_num) {
  if (is_inited_) {
    return 0;
  }

  int err;
  if ((err = snd_pcm_open(capture_handle, card_name, SND_PCM_STREAM_CAPTURE,
                          0)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot open audio device: " << card_name
                            << ", err is " << snd_strerror(err);
    return -1;
  }

  snd_pcm_hw_params_alloca(params);

  if ((err = snd_pcm_hw_params_any(*capture_handle, *params)) < 0) {
    LOGE_T(kAudioRecordTag)
        << "cannot initialize hardware parameter structure err is "
        << snd_strerror(err);
    return -1;
  }

  if ((err = snd_pcm_hw_params_set_access(*capture_handle, *params,
                                          SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot set access type, err is "
                            << snd_strerror(err);
    return -1;
  }

  LOGI_T(kAudioRecordTag) << "hw_params access setted";

  if ((err = snd_pcm_hw_params_set_format(*capture_handle, *params,
                                          ref_pcm_format_)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot set ref sample format, err is "
                            << snd_strerror(err);
    return -1;
  }

  LOGI_T(kAudioRecordTag) << "format set finished";

  if ((err = snd_pcm_hw_params_set_rate_near(*capture_handle, *params, rate,
                                             nullptr)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot set sample rate, err is "
                            << snd_strerror(err);
    return -1;
  }

  LOGI_T(kAudioRecordTag) << "rate set finished";

  if ((err = snd_pcm_hw_params_set_channels(*capture_handle, *params,
                                            channel_num)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot set channel count, err is "
                            << snd_strerror(err);
    return -1;
  }

  snd_pcm_uframes_t frame = 16 * 30;

  err = snd_pcm_hw_params_set_period_size(*capture_handle, *params, frame, 0);
  if (err < 0) {
    LOGE_T(kAudioRecordTag) << "Set period size failed: ", snd_strerror(err);
    return err;
  }

  /* request period size in bytes */
  err = snd_pcm_hw_params_get_period_size(*params, &record_params->period_size,
                                          0);

  if (err < 0) {
    LOGE_T(kAudioRecordTag) << "Unable to get period size: "
                            << snd_strerror(err);
    return -1;
  }

  LOGI_T(kAudioRecordTag) << "period size is " << record_params->period_size;

  if ((err = snd_pcm_hw_params(*capture_handle, *params)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot set parameters, err is "
                            << snd_strerror(err);
    return -1;
  }

  if ((err = snd_pcm_prepare(*capture_handle)) < 0) {
    LOGE_T(kAudioRecordTag) << "cannot prepare audio interface , err "
                            << snd_strerror(err);
    return -1;
  }

  LOGI_T(kAudioRecordTag) << "buffer allocated";

  return 0;
}

int AudioRecordHandler::StopAudioRecoding() {
  if (!audio_start_flag) return 0;
  audio_start_flag = false;

  //  pthread_join(mic_record_thread, nullptr);
  pthread_join(ref_record_thread, nullptr);
  return 0;
}

#if 0
int AudioRecordHandler::RegisterMicCallback(AudioMicDataCallback callback) {
  if (callback == nullptr) {
    LOGE_T(kAudioRecordTag) << "input callback is null";
    return -1;
  }
  audio_mic_callback = callback;
  return 0;
}
#endif

int AudioRecordHandler::RegisterRefCallback(AudioRefDataCallback callback) {
  if (callback == nullptr) {
    LOGE_T(kAudioRecordTag) << "input callback is null";
    return -1;
  }
  audio_ref_callback = callback;
  return 0;
}

void AudioRecordHandler::ReleaseMemory(AudioRecordParams *params) {
  if (params != nullptr) {
    if (params->capture_handle != nullptr) {
      snd_pcm_close(params->capture_handle);
      snd_pcm_hw_free(params->capture_handle);
      params->capture_handle = nullptr;
    }

    if (params->record_params != nullptr) {
      snd_pcm_hw_params_free(params->record_params);
      params->record_params = nullptr;
    }
  }
}

int AudioRecordHandler::DestroyRecordHandler() {
  if (ref_audio_record_params != nullptr) {
    ReleaseMemory(ref_audio_record_params);

    delete ref_audio_record_params;
    ref_audio_record_params = nullptr;
  }

  is_inited_ = false;

  return 0;
}

}  // namespace kws_ztx
