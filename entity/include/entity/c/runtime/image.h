/**
 * @file image.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef RUNTIME_IMAGE_H
#define RUNTIME_IMAGE_H

#include <stdint.h>
#include <library/string/fixed_string.h>


typedef
enum image_format_t {
  RUNTIME_IMAGE_FORMAT_RGBA,
  RUNTIME_IMAGE_FORMAT_BGRA,
  RUNTIME_IMAGE_FORMAT_RGB,
  RUNTIME_IMAGE_FORMAT_BGR,
  RUNTIME_IMAGE_FORMAT_LA,             // luminance/alpha
  RUNTIME_IMAGE_FORMAT_L,
  RUNTIME_IMAGE_FORMAT_A
} image_format_t;

// TODO: currently for image/font we are setting the path first and only filling
// out the rest of the members after the loaders have been called. I want to 
// remove this 2 step process (callbacks is a good solution to this).
typedef
struct image_t {
  fixed_str_t path;
  uint32_t width;
  uint32_t height;
  image_format_t format;
  uint8_t* buffer;
  size_t buffer_size;
} image_t;

#endif