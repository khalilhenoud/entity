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
#include <entity/internal/module.h>


typedef struct font_runtime_t font_runtime_t;
typedef struct allocator_t allocator_t;
typedef struct font_t font_t;
typedef float glyph_bounds_t[6];

ENTITY_API
font_runtime_t*
create_font_runtime(
  const font_t* font,
  const allocator_t* allocator);

ENTITY_API
void
free_font_runtime(
  font_runtime_t* font_runtime,
  const allocator_t* allocator);

ENTITY_API
void
free_font_runtime_internal(
  font_runtime_t* font_runtime,
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
uint32_t
get_glyph_count(const font_runtime_t* font);

ENTITY_API
uint32_t
get_font_column_count(const font_runtime_t* font);

ENTITY_API
uint32_t
get_font_row_count(const font_runtime_t* font);

ENTITY_API
void
get_glyph_bounds(
  const font_runtime_t* font,
  const char c,
  glyph_bounds_t *out);

ENTITY_API
uint32_t
has_glyph(
  const font_runtime_t* font,
  const char c);

#ifdef __cplusplus
}
#endif

#endif