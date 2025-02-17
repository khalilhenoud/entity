/**
 * @file material_utils.c
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
#include <entity/c/mesh/material.h>
#include <entity/c/mesh/material_utils.h>


material_t* 
allocate_material_array(
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(count && "Passed count is 0!");
  assert(allocator);

  {
    material_t* ptr = 
      (material_t*)allocator->mem_cont_alloc(count, sizeof(material_t));
    assert(ptr && "Failed to allocate!");
    return ptr;
  }
}

void
free_material_array(
  material_t* material, 
  uint32_t count,
  const allocator_t* allocator)
{
  assert(count && "Passed count is 0!");
  assert(allocator);

  for (uint32_t i = 0; i < count; ++i)
    free_material_internal(material + i, allocator);

  allocator->mem_free(material);
}

void
free_material_internal(
  material_t* material, 
  const allocator_t* allocator)
{
  assert(material && allocator);

  cstring_cleanup(material->name, NULL);
  allocator->mem_free(material->name);
}

void
free_material(
  material_t* material, 
  const allocator_t *allocator)
{
  free_material_internal(material, allocator);
  allocator->mem_free(material);
}