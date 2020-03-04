#define LOG_TAG "TestWavFile"

#include "audiofile.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT 0x20746d66
#define ID_DATA 0x61746164

#define FORMAT_PCM 1

struct wav_file* create_wav_file(uint32_t sample_rate, int channel_count,
                                 char* file_name) {
  struct wav_file* pfile = NULL;
  struct wav_header* pheader = NULL;

  if ((channel_count < 1) || (channel_count > 2)) {
    printf("channel_count error --> create wav file fail\n");
    return 0;
  }

  switch (sample_rate) {
    case 8000:
    case 11025:
    case 16000:
    case 22050:
    case 24000:
    case 32000:
    case 44100:
    case 48000:
      break;

    default:
      printf("sample_rate error --> create wav file fail\n");
      return 0;
  }

  pheader = (struct wav_header*)malloc(sizeof(struct wav_header));

  if (!pheader) {
    printf("malloc error --> create wav file fail\n");
    return 0;
  }

  pfile = (struct wav_file*)malloc(sizeof(struct wav_file));

  if (!pfile) {
    free(pheader);
    printf("malloc error --> create wav file fail\n");
    return 0;
  }

  pfile->file = fopen(file_name, "wb");

  if (!pfile->file) {
    free(pheader);
    free(pfile);
    printf("fopen error --> create wav file fail\n");
    return 0;
  }

  pfile->header = pheader;
  pfile->file_frames = 0;

  pheader->riff_id = ID_RIFF;
  pheader->riff_sz = 0;
  pheader->riff_fmt = ID_WAVE;
  pheader->fmt_id = ID_FMT;
  pheader->fmt_sz = 16;
  pheader->audio_format = FORMAT_PCM;
  pheader->num_channels = channel_count;
  pheader->sample_rate = sample_rate;
  pheader->bits_per_sample = 16;
  pheader->byte_rate = (pheader->bits_per_sample / 8) * pheader->num_channels *
                       pheader->sample_rate;
  pheader->block_align = pheader->num_channels * (pheader->bits_per_sample / 8);
  pheader->data_id = ID_DATA;

  fseek(pfile->file, sizeof(struct wav_header), SEEK_SET);
  return pfile;
}

uint32_t write_wav_file(struct wav_file* pfile, void* buffer, ssize_t frames) {
  ssize_t frames_w;

  if (!pfile || !buffer) {
    printf("wav_file is NULL\n");
    return 0;
  }

  frames_w = fwrite(buffer, pfile->header->block_align, frames, pfile->file);

  if (frames != frames_w) {
    printf("fwrite error!!\n");
  }

  pfile->file_frames += frames_w;

  return frames_w;
}

void release_wav_file(struct wav_file* pfile) {
  if (pfile) {
    if (pfile->file && pfile->header) {
      pfile->header->data_sz = pfile->file_frames * pfile->header->block_align;
      fseek(pfile->file, 0, SEEK_SET);
      fwrite(pfile->header, sizeof(struct wav_header), 1, pfile->file);
      printf("save wav file ok!!\n");
      fclose(pfile->file);
      free(pfile->header);
      free(pfile);
    }
  }
}
