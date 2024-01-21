/**
 * @file light_utils.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <assert.h>
#include <library/string/string.h>
#include <library/allocator/allocator.h>
#include <entity/c/scene/light.h>
#include <entity/c/scene/light_utils.h>


light_t*
allocate_light_array(
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(count && allocator);

  {
    light_t* ptr = (light_t*)allocator->mem_cont_alloc(count, sizeof(light_t));
    assert(ptr);
    memset(ptr, 0, sizeof(light_t));
    return ptr;
  }
}

void
free_light_internal(
  light_t* light, 
  const allocator_t* allocator)
{
  assert(light && allocator);

  if (light->name)
    free_string(light->name);
}

void
free_light_array(
  light_t* light, 
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(light && count && allocator);
  
  for (uint32_t i = 0; i < count; ++i)
    free_light_internal(light + i, allocator);

  allocator->mem_free(light);
}