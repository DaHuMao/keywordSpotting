/*****************************************************************************
 * Created by tongxiao.zhang
 * Date : 2019/12/17
 *****************************************************************************/

#ifndef SRC_RECORD_TINYALSA_RECORD_H_
#define SRC_RECORD_TINYALSA_RECORD_H_

#include "tinyalsa/asoundlib.h"
#include "util/circ_buff.h"

typedef void (*record_Callback)(const char *buf, const size_t read_len);

namespace kws_ztx {

class AudioRecordTiny {
 public:
  AudioRecordTiny();
  ~AudioRecordTiny();

  static AudioRecordTiny &instance() { return audio_handle; }

  int record_init(size_t rate, size_t channels, record_Callback callback_fun);
  int record_destroy();

 private:
  static void *record_thread(void *arg);
  int record_process();
  record_Callback record_callback;
  struct pcm_config config;
  struct pcm *pcm;
  char *buffer;
  size_t _size, _rate, _channels;
  static AudioRecordTiny audio_handle;
  pthread_t audio_tid;
};  // class 

}  // kws_ztx


#endif  // SRC_RECORD_TINYALSA_RECORD_H_
