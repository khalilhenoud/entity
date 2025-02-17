/**
 * @file texture_utils.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-12-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include <string.h>
#include <library/allocator/allocator.h>
#include <library/string/cstring.h>
#include <entity/c/mesh/texture.h>
#include <entity/c/mesh/texture_utils.h>


texture_t* 
allocate_texture_array(
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(count && "Passed count is 0!");
  assert(allocator);

  {
    texture_t* ptr = 
      (texture_t*)allocator->mem_cont_alloc(count, sizeof(texture_t));
    assert(ptr && "Failed to allocate!");
    return ptr;
  }
}

void
free_texture_array(
  texture_t* texture, 
  uint32_t count,
  const allocator_t* allocator)
{
  assert(count && "Passed count is 0!");
  assert(allocator);

  for (uint32_t i = 0; i < count; ++i)
    free_texture_internal(texture + i, allocator);

  allocator->mem_free(texture);
}

void
free_texture_internal(
  texture_t* texture, 
  const allocator_t* allocator)
{
  assert(texture && allocator);

  cstring_cleanup(texture->path, NULL);
  allocator->mem_free(texture->path);
}

void
free_texture(
  texture_t* texture, 
  const allocator_t *allocator)
{
  free_texture_internal(texture, allocator);
  allocator->mem_free(texture);
}