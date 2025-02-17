/**
 * @file font.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MISC_FONT_H
#define MISC_FONT_H


typedef struct cstring_t cstring_t;

typedef
struct font_t {
  cstring_t *image_file;
  cstring_t *data_file;
} font_t;

#endif