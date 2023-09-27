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


font_t*
create_font(
  const char *image_file, 
  const char *data_file, 
  const allocator_t* allocator)
{
  assert(allocator != NULL);
  assert(
    image_file != NULL && 
    data_file != NULL && 
    "image file or data file are NULL!");
  
  {
    font_t* font = (font_t*)allocator->mem_alloc(sizeof(font_t));
    memset(font, 0, sizeof(font_t));
    assert(
      strlen(image_file) < sizeof(font->image_file.data) && 
      "image_file path does not fit the fixed_str_t data struct!");
    assert(
      strlen(data_file) < sizeof(font->data_file.data) && 
      "data_file path does not fit the fixed_str_t data struct!");
    memcpy(font->image_file.data, image_file, strlen(image_file));
    memcpy(font->data_file.data, data_file, strlen(data_file));
    return font;
  }
}

void
free_font(
  font_t* font, 
  const allocator_t* allocator)
{
  assert(allocator != NULL);
  assert(font != NULL && "font is NULL!");
  allocator->mem_free(font);
}

////////////////////////////////////////////////////////////////////////////////
uint32_t
get_glyph_count(const font_t* font)
{
  return 
    (font->image_width/font->cell_width) * 
    (font->image_height/font->cell_height);
}

uint32_t
get_font_column_count(const font_t* font)
{
  return font->image_width/font->cell_width;
}

uint32_t
get_font_row_count(const font_t* font)
{
  return font->image_height/font->cell_height;
}

void
get_glyph_bounds(
  const font_t* font, 
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
  const font_t* font, 
  const char c)
{
 uint32_t total = get_glyph_count(font);
 uint32_t cui = (uint32_t)c;
 return cui >= font->start_char && cui < (font->start_char + total); 
}