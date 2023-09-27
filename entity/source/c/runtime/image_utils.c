/**
 * @file image_utils.c
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
#include <library/allocator/allocator.h>
#include <entity/c/runtime/image.h>
#include <entity/c/runtime/image_utils.h>


image_t*
create_image(const char* path, const allocator_t* allocator)
{
  assert(path && "path cannot be NULL");
  assert(allocator && "allocator cannot be NULL");

  {
    image_t* image = (image_t*)allocator->mem_alloc(sizeof(image_t));
    memset(image, 0, sizeof(image_t));
    memcpy(image->path.data, path, strlen(path));
    return image;
  }
}

void
free_image(image_t* image, const allocator_t* allocator)
{
  assert(image && "image cannot be NULL");
  assert(allocator && "allocator cannot be NULL");

  // attempt to free the buffer.
  if (image->buffer) {
    assert(
      image->buffer_size != 0 && 
      "if buffer is non-null the size cannot be 0!");

    allocator->mem_free(image->buffer);
  }

  allocator->mem_free(image);
}

void
allocate_image_buffer(
  image_t* image, 
  const size_t buffer_size, 
  const allocator_t* allocator)
{
  assert(allocator && "allocator cannot be NULL");
  assert(image && "image cannot be NULL");
  assert(
    image->buffer == NULL && image->buffer_size == 0 &&
    "buffer is not emtpy!");
  
  image->buffer_size = buffer_size;
  image->buffer = (uint8_t *)allocator->mem_alloc(buffer_size);
}

void
free_image_buffer(image_t* image, const allocator_t* allocator)
{
  assert(image && "image cannot be NULL");
  assert(
    image->buffer != NULL && image->buffer_size != 0 &&
    "attempting to free a NULL image buffer!");
  assert(allocator && "allocator cannot be NULL");

  // NOTE: The image might be reused/reloaded, so we reset the buffer variables.
  allocator->mem_free(image->buffer);
  image->buffer = NULL;
  image->buffer_size = 0;
}

////////////////////////////////////////////////////////////////////////////////
uint32_t
get_image_component_count(const image_t* image)
{
  switch (image->format) {
    case RUNTIME_IMAGE_FORMAT_RGBA:
    case RUNTIME_IMAGE_FORMAT_BGRA:
      return 4;
      break;
    case RUNTIME_IMAGE_FORMAT_RGB:
    case RUNTIME_IMAGE_FORMAT_BGR:
      return 3;
      break;
    case RUNTIME_IMAGE_FORMAT_LA:
      return 2;
      break;
    case RUNTIME_IMAGE_FORMAT_L:
    case RUNTIME_IMAGE_FORMAT_A:
      return 1;
      break;
  }

  assert(0 && "Format is unsupported");
  return 0;
}

void
get_image_extension(const image_t* image, uint8_t extension[8])
{
  uint8_t* at = strrchr(image->path.data, '.');
  assert(at && "File has no extension '.' separator!");
  assert(strlen(at + 1) < 8 && "Extension is longer than 8 character!");

  ++at;
  memset(extension, 0, sizeof(extension));
  memcpy(extension, at, strlen(at));
}