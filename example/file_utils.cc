//
// Created by 崔学成 on 2019-06-12.
//

#include "file_utils.h"
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <vector>

int GetWaveHeadLen(hdr_t *wave_head, FILE *fd) {
  int head_len = 0;
  int i = 0;
  char str[8];

  fseek(fd, 0L, SEEK_SET);

  if (1 != fread(wave_head->riff, 4, 1, fd)) return -1;
  head_len += 4;
  if (1 != fread(&wave_head->totsize, sizeof(int), 1, fd)) return -1;
  head_len += sizeof(int);
  if (1 != fread(wave_head->wave, 4, 1, fd)) return -1;
  head_len += 4;
  if (1 != fread(wave_head->fmtstr, 4, 1, fd)) return -1;
  head_len += 4;
  if (1 != fread(&wave_head->dum1, sizeof(int), 1, fd)) return -1;
  head_len += sizeof(int);
  if (1 != fread(&wave_head->dum2, sizeof(short), 1, fd)) return -1;
  head_len += sizeof(short);
  if (1 != fread(&wave_head->numchans, sizeof(short), 1, fd)) return -1;
  head_len += sizeof(short);
  if (1 != fread(&wave_head->Fs, sizeof(int), 1, fd)) return -1;
  head_len += sizeof(int);
  if (1 != fread(&wave_head->nbytespersec, sizeof(int), 1, fd)) return -1;
  head_len += sizeof(int);
  if (1 != fread(&wave_head->nbytespersample, sizeof(short), 1, fd)) return -1;
  head_len += sizeof(short);
  if (1 != fread(&wave_head->nbitspersample, sizeof(short), 1, fd)) return -1;
  head_len += sizeof(short);

  i = 0;
  while (i++ < wave_head->dum1 - 16) {
    if (1 != fread(str, 1, 1, fd)) return -1;
  }

  head_len += (wave_head->dum1 - 16);

  if (1 != fread(wave_head->data, 4, 1, fd)) return -1;
  head_len += 4;

  if (0 == strncmp(wave_head->data, "fact", 4)) {
    if (1 != fread(str, 8, 1, fd)) return -1;
    head_len += 8;
    if (1 != fread(wave_head->data, 4, 1, fd)) return -1;
    head_len += 4;
  }

  if (1 != fread(&wave_head->datsize, sizeof(int), 1, fd)) return -1;
  head_len += sizeof(int);

  if (strncmp(wave_head->riff, "RIFF", 4) != 0 ||
      strncmp(wave_head->wave, "WAVE", 4) ||
      strncmp(wave_head->data, "data", 4)) {
    fprintf(stderr, "stream is not a WAVE format\n");
    fprintf(stderr, "%s %s %s %d(%d - %d) == %d\n", wave_head->riff,
            wave_head->wave, wave_head->data,
            wave_head->totsize - wave_head->datsize, wave_head->totsize,
            wave_head->datsize, head_len);
    return (-1);
  }

  return (head_len);
}

int CheckFileExists(const char *file_name) {
  if (nullptr == file_name) return -1;

  if (access(file_name, F_OK) != 0) return -1;

  return 0;
}

int CheckDirExists(const char *path_name) {
  if (nullptr == path_name) return -1;

  DIR *dir;

  if ((dir = opendir(path_name)) == nullptr) return -1;

  closedir(dir);

  return 0;
}

int CreateDir(const char *path_name) {
  if (nullptr == path_name) return -1;

  return mkdir(path_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

std::string GenerateFileSerialNum(int num) {
  std::string number = "" /*std::to_string(num)*/;

  switch (number.size()) {
    case 1:
      number = "00" + number;
      break;
    case 2:
      number = "0" + number;
      break;
    case 3:
      break;
    default:
      break;
  }

  return std::move(number);
}

long long GetTimeStamp(const char *date) {
  char copy_str[strlen(date) + 1];
  strcpy(copy_str, date);

  std::vector<std::string> list;

  const char *split_char = "-";

  char *saver = nullptr;

  char *result = strtok_r(copy_str, split_char, &saver);

  while (result != NULL) {
    list.emplace_back(result);

    result = strtok_r(NULL, split_char, &saver);
  }
  tm stm;

  memset(&stm, 0, sizeof(stm));

  if (list.size() < 7) {
    return -1;
  }

  int year = (int)strtol(list[0].c_str(), nullptr, 0);
  int month = (int)strtol(list[1].c_str(), nullptr, 0);
  int day = (int)strtol(list[2].c_str(), nullptr, 0);
  int hour = (int)strtol(list[3].c_str(), nullptr, 0);
  int min = (int)strtol(list[4].c_str(), nullptr, 0);
  int sec = (int)strtol(list[5].c_str(), nullptr, 0);

  stm.tm_year = year - 1900;
  stm.tm_mon = month - 1;
  stm.tm_mday = day;
  stm.tm_hour = hour;
  stm.tm_min = min;
  stm.tm_sec = sec;

  //  printf("%d-%0d-%0d %0d:%0d:%0d\n", year, month, day, hour, min, sec);

  long long date_time = mktime(&stm) * 1000;

  if (list[6].find("kHZ") == -1) {
    int milli = (int)strtol(list[6].c_str(), nullptr, 0);
    date_time += milli;
  }

  return date_time;
}
