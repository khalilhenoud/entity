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