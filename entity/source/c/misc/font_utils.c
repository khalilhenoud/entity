/**
 * @file font_utils.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include <string.h>
#include <library/allocator/allocator.h>
#include <library/string/string.h>
#include <entity/c/misc/font.h>
#include <entity/c/misc/font_utils.h>


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
    font->image_file = allocate_string(image_file, allocator);
    font->data_file = allocate_string(data_file, allocator);
    return font;
  }
}

font_t* 
allocate_font_array(
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(count && "do not allocate array of size 0!");
  assert(allocator);

  {
    font_t* fonts = (font_t*)allocator->mem_cont_alloc(count, sizeof(font_t));
    assert(fonts && "Failed to allocate memory");
    return fonts;
  }
}

void
free_font_array(
  font_t* fonts, 
  uint32_t count,
  const allocator_t* allocator)
{
  assert(fonts && count && "trying to free an invalid array!");
  assert(allocator);

  for (uint32_t i = 0; i < count; ++i)
    free_font_internal(fonts + i, allocator);

  allocator->mem_free(fonts);
}

void
free_font_internal(
  font_t* font, 
  const allocator_t* allocator)
{
  assert(allocator != NULL);
  assert(font != NULL && "font is NULL!");
  free_string(font->image_file);
  free_string(font->data_file);
}

void
free_font(
  font_t* font, 
  const allocator_t* allocator)
{
  free_font_internal(font, allocator);
  allocator->mem_free(font);
}