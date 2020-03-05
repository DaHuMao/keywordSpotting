#ifndef EXAMPLE_AUDIOFILE_H_

#define EXAMPLE_AUDIOFILE_H_

#ifdef __cplusplus
extern "C" {
#endif
struct wav_header {
  uint32_t riff_id;
  uint32_t riff_sz;
  uint32_t riff_fmt;
  uint32_t fmt_id;
  uint32_t fmt_sz;
  uint16_t audio_format;
  uint16_t num_channels;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t block_align;
  uint16_t bits_per_sample;
  uint32_t data_id;
  uint32_t data_sz;
};

struct wav_file {
  FILE *file;
  struct wav_header *header;
  uint32_t file_frames;
};

void release_wav_file(struct wav_file *);
uint32_t write_wav_file(struct wav_file *, void *buffer, ssize_t frames);
struct wav_file *create_wav_file(uint32_t sample_rate, int channel_count,
                                 char *file_name);

#ifdef __cplusplus
}
#endif
#endif  // EXAMPLE_AUDIOFILE_H_
