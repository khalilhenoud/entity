/**
 * @file font.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef RUNTIME_FONT_H
#define RUNTIME_FONT_H

#include <stdint.h>
#include <entity/misc/font.h>

#define FONT_GLYPH_COUNT 256


typedef float glyph_bounds_t[6];

typedef
struct glyph_info_t {
  uint32_t x, y;
  uint32_t width;
  uint32_t width_offset;
} glyph_info_t;

typedef
struct font_runtime_t {
  // TODO(khalil): Should this be a copy or a pointer/handle/index.
  font_t font;
  uint32_t image_width, image_height;
  uint32_t cell_width, cell_height;
  uint32_t font_width, font_height;
  uint32_t start_char;

  glyph_info_t glyphs[FONT_GLYPH_COUNT];
  glyph_bounds_t bounds[FONT_GLYPH_COUNT];
} font_runtime_t;



#endif