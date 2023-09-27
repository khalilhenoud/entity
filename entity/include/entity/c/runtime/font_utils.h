/**
 * @file font_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef RUNTIME_FONT_UTILS_H
#define RUNTIME_FONT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <library/allocator/allocator.h>
#include <entity/c/internal/module.h>


typedef struct font_t font_t;

ENTITY_API
font_t*
create_font(
  const char *image_file, 
  const char *data_file, 
  const allocator_t* allocator);

ENTITY_API
void
free_font(
  font_t* font, 
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
uint32_t
get_glyph_count(const font_t* font);

ENTITY_API
uint32_t
get_font_column_count(const font_t* font);

ENTITY_API
uint32_t
get_font_row_count(const font_t* font);

ENTITY_API
void
get_glyph_bounds(
  const font_t* font, 
  const char c, 
  glyph_bounds_t *out);

ENTITY_API
uint32_t 
has_glyph(
  const font_t* font, 
  const char c);

#ifdef __cplusplus
}
#endif

#endif