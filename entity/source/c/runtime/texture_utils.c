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
#include <library/string/cstring.h>
#include <entity/c/mesh/texture.h>
#include <entity/c/runtime/texture.h>
#include <entity/c/runtime/texture_utils.h>


texture_runtime_t*
create_texture_runtime(
  const texture_t* texture, 
  const allocator_t* allocator)
{
  assert(texture && allocator);

  {
    texture_runtime_t* runtime = 
      (texture_runtime_t*)allocator->mem_alloc(sizeof(texture_runtime_t));
    memset(runtime, 0, sizeof(texture_runtime_t));
    
    cstring_def(&runtime->texture.path);
    cstring_setup(&runtime->texture.path, texture->path.str, allocator);

    return runtime;
  }
}

void
free_texture_runtime(
  texture_runtime_t* runtime, 
  const allocator_t* allocator)
{
  free_texture_runtime_internal(runtime, allocator);
  allocator->mem_free(runtime);
}

void
free_texture_runtime_internal(
  texture_runtime_t* runtime, 
  const allocator_t* allocator)
{
  assert(runtime && allocator);

  // attempt to free the buffer.
  if (runtime->buffer) {
    assert(
      runtime->buffer_size != 0 && 
      "if buffer is non-null the size cannot be 0!");

    allocator->mem_free(runtime->buffer);
  }

  cstring_cleanup2(&runtime->texture.path);
}

void
allocate_runtime_buffer(
  texture_runtime_t* runtime, 
  const size_t buffer_size, 
  const allocator_t* allocator)
{
  assert(allocator && "allocator cannot be NULL");
  assert(runtime && "image cannot be NULL");
  assert(
    runtime->buffer == NULL && runtime->buffer_size == 0 &&
    "buffer is not emtpy!");
  
  runtime->buffer_size = buffer_size;
  runtime->buffer = (uint8_t *)allocator->mem_alloc(buffer_size);
}

void
free_runtime_buffer(
  texture_runtime_t* runtime, 
  const allocator_t* allocator)
{
  assert(runtime && "runtime cannot be NULL");
  assert(
    runtime->buffer != NULL && runtime->buffer_size != 0 &&
    "attempting to free a NULL runtime buffer!");
  assert(allocator && "allocator cannot be NULL");

  // NOTE: The runtime might be reused/reloaded, so we reset the buffer
  // variables.
  allocator->mem_free(runtime->buffer);
  runtime->buffer = NULL;
  runtime->buffer_size = 0;
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
  uint8_t* at = strrchr(image->texture.path.str, '.');
  assert(at && "File has no extension '.' separator!");
  assert(strlen(at + 1) < 8 && "Extension is longer than 8 character!");

  ++at;
  memset(extension, 0, sizeof(extension));
  memcpy(extension, at, strlen(at));
}