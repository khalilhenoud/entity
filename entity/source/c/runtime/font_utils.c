/**
 * @file font_utils.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <assert.h>
#include <string.h>
#include <entity/c/runtime/font.h>
#include <entity/c/runtime/font_utils.h>
#include <library/allocator/allocator.h>
#include <library/string/cstring.h>


font_runtime_t*
create_font_runtime(
  const font_t* font,
  const allocator_t* allocator)
{
  assert(allocator && font);

  {
    font_runtime_t* runtime =
      (font_runtime_t*)allocator->mem_alloc(sizeof(font_runtime_t));
    memset(runtime, 0, sizeof(font_runtime_t));

    cstring_setup(&runtime->font.image_file, font->image_file.str, allocator);
    cstring_setup(&runtime->font.data_file, font->data_file.str, allocator);

    return runtime;
  }
}

void
free_font_runtime(
  font_runtime_t* runtime,
  const allocator_t* allocator)
{
  free_font_runtime_internal(runtime, allocator);
  allocator->mem_free(runtime);
}

void
free_font_runtime_internal(
  font_runtime_t* runtime,
  const allocator_t* allocator)
{
  assert(allocator != NULL);
  assert(runtime != NULL && "runtime is NULL!");

  cstring_cleanup2(&runtime->font.image_file);
  cstring_cleanup2(&runtime->font.data_file);
}

////////////////////////////////////////////////////////////////////////////////
uint32_t
get_glyph_count(const font_runtime_t* font)
{
  return
    (font->image_width/font->cell_width) *
    (font->image_height/font->cell_height);
}

uint32_t
get_font_column_count(const font_runtime_t* font)
{
  return font->image_width/font->cell_width;
}

uint32_t
get_font_row_count(const font_runtime_t* font)
{
  return font->image_height/font->cell_height;
}

void
get_glyph_bounds(
  const font_runtime_t* font,
  const char c,
  glyph_bounds_t *out)
{
  assert(font != NULL);

  {
    char glyph = has_glyph(font, c) ? c : '$';
    memcpy(out, font->bounds + glyph, sizeof(glyph_bounds_t));
  }
}

uint32_t
has_glyph(
  const font_runtime_t* font,
  const char c)
{
 uint32_t total = get_glyph_count(font);
 uint32_t cui = (uint32_t)c;
 return cui >= font->start_char && cui < (font->start_char + total);
}