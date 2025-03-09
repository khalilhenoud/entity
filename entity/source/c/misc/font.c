/**
 * @file font.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2025-02-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <assert.h>
#include <string.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <library/string/cstring.h>
#include <entity/c/misc/font.h>


void
font_def(void *ptr)
{
  assert(ptr);

  {
    font_t *font = (font_t *)ptr;
    memset(font, 0, sizeof(font_t));
  }
}

uint32_t 
font_is_def(const void *ptr)
{
  assert(ptr);

  {
    const font_t *font = (const font_t *)ptr;
    return 
      font->image_file == NULL && 
      font->data_file == NULL;
  }
}

void 
font_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const font_t *font = (const font_t *)src;
    cstring_serialize(font->image_file, stream);
    cstring_serialize(font->data_file, stream);
  }
}

void 
font_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    font_t *font = (font_t *)dst;
    font->image_file = (cstring_t *)allocator->mem_alloc(sizeof(cstring_t));
    cstring_def(font->image_file);
    cstring_deserialize(font->image_file, allocator, stream);

    font->data_file = (cstring_t *)allocator->mem_alloc(sizeof(cstring_t));
    cstring_def(font->data_file);
    cstring_deserialize(font->data_file, allocator, stream);
  }
}

size_t 
font_type_size(void)
{
  return sizeof(font_t);
}

uint32_t 
font_owns_alloc(void)
{
  return 0;
}

const allocator_t *
font_get_alloc(const void *ptr)
{
  return NULL;
}

void 
font_cleanup(
  void *ptr, 
  const allocator_t *allocator)
{
  assert(ptr && !font_is_def(ptr));
  assert(allocator);

  {
    font_t *font = (font_t *)ptr;
    cstring_free(font->image_file, allocator);
    cstring_free(font->data_file, allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
void
font_setup(
  font_t *font, 
  const char *image_file, 
  const char *data_file, 
  const allocator_t* allocator)
{
  assert(allocator);
  assert(font && font_is_def(font));
  assert(
    image_file && 
    data_file && 
    "image file or data file are NULL!");

  font->image_file = cstring_create(image_file, allocator);
  font->data_file = cstring_create(data_file, allocator);
}

font_t*
font_create(
  const char *image_file, 
  const char *data_file, 
  const allocator_t *allocator)
{
  assert(allocator);
  assert(
    image_file && 
    data_file && 
    "image file or data file are NULL!");

  {
    font_t *font = (font_t *)allocator->mem_alloc(sizeof(font_t));
    font_def(font);
    font_setup(font, image_file, data_file, allocator);
    return font;
  }
}

void
font_free(
  font_t *font, 
  const allocator_t *allocator)
{
  font_cleanup(font, allocator);
  allocator->mem_free(font);
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_font_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = font_def;
  vtable.fn_is_def = font_is_def;
  vtable.fn_serialize = font_serialize;
  vtable.fn_deserialize = font_deserialize;
  vtable.fn_type_size = font_type_size;
  vtable.fn_owns_alloc = font_owns_alloc;
  vtable.fn_get_alloc = font_get_alloc;
  vtable.fn_cleanup = font_cleanup;
  register_type(get_type_id(font_t), &vtable);
}