//
// Created by 崔学成 on 2019-06-12.
//

#ifndef HOBOTNLP_FILE_UTILS_H_
#define HOBOTNLP_FILE_UTILS_H_
/*
 * Copyright (c) 2019 horizon.ai.
 *
 * Unpublished copyright. All rights reserved. This material contains
 * proprietary information that should be used or copied only within
 * horizon.ai, except with written permission of horizon.ai.
 * @author: 崔学成
 * @file: file_utils.h
 * @date: 2019-06-12
 * @brief:
 */

#include <cstdio>
#include <string>

typedef struct hdr {
  char riff[4];           // String 'RIFF' without \0
  int totsize;            // Total file size - 8
  char wave[4];           // String 'WAVE' without \0
  char fmtstr[4];         // String 'fmt_' without \0
  int dum1;               // Length of format chunk (0x10)
  short dum2;             // Always 0x01
  short numchans;         // Number of channels
  int Fs;                 // Sample rate (in Hz)
  int nbytespersec;       // number of bytes/second
  short nbytespersample;  // number of bytes of all samples
  // (1=8 bit mono, 2=8 bit stereo or 16 bit mono, 4= 16 bit stereo)
  short nbitspersample;  // number of bits/samples
  char data[4];          // string 'data' without \0
  unsigned int datsize;  // number of data bytes (not samples)
} hdr_t;

int GetWaveHeadLen(hdr_t *wave_head, FILE *fd);

/**
 * 判断文件是否存在
 * @param file_name
 * @return
 */
int CheckFileExists(const char *file_name);

/**
 * 判断文件夹是否存在
 * @param path_name
 * @return
 */
int CheckDirExists(const char *path_name);

/**
 * 创建目录
 * @param path_name
 * @return
 */
int CreateDir(const char *path_name);

/**
 * 产生文件序列号, 比如输入0, 则输出为000; 输入为1, 输入为001, 默认三位
 * @param num
 * @return
 */
std::string GenerateFileSerialNum(int num);

/**
 * 根据日期获得对应的时间戳
 * @param date
 * @return
 */
long long GetTimeStamp(const char *date);

#endif  // HOBOTNLP_FILE_UTILS_H_
