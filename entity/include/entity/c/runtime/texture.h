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
#include <entity/c/mesh/texture.h>


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

typedef
struct texture_runtime_t {
  // TODO(khalil): should this be a pointer of an index, or as is.
  texture_t texture;
  uint32_t width;
  uint32_t height;
  image_format_t format;
  uint8_t* buffer;
  size_t buffer_size;
} texture_runtime_t;

typedef texture_runtime_t image_t;

#endif