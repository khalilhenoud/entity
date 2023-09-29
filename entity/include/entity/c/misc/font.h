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

#include <library/string/fixed_string.h>


typedef
struct font_t {
  fixed_str_t image_file;
  fixed_str_t data_file;
} font_t;

#endif