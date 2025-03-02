/**
 * @file bvh_utils.c
 * @author khalilhenoud@gmail.com
 * @brief non-general bvh for simplistic collision detection.
 * @version 0.1
 * @date 2023-10-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <math/c/face.h>
#include <entity/c/spatial/bvh.h>
#include <entity/c/spatial/bvh_utils.h>
#include <library/allocator/allocator.h>


bvh_t* 
allocate_bvh_array(
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(count && "Passed count is 0!");
  assert(allocator);

  {
    bvh_t* ptr = (bvh_t*)allocator->mem_cont_alloc(count, sizeof(bvh_t));
    assert(ptr && "Failed to allocate!");
    return ptr;
  }
}

void
free_bvh_array(
  bvh_t* bvh, 
  uint32_t count,
  const allocator_t* allocator)
{
  assert(count && "Passed count is 0!");
  assert(allocator);

  for (uint32_t i = 0; i < count; ++i)
    free_bvh_internal(bvh + i, allocator);

  allocator->mem_free(bvh);
}

void
free_bvh_internal(
  bvh_t* bvh, 
  const allocator_t* allocator)
{
  assert(bvh && allocator);

  // we might have moved the pointers to avoid duplicates.
  if (bvh->faces)
    allocator->mem_free(bvh->faces);
  if (bvh->normals)
    allocator->mem_free(bvh->normals);
  if (bvh->bounds)
    allocator->mem_free(bvh->bounds);
  if (bvh->nodes)
    allocator->mem_free(bvh->nodes);
}

void
free_bvh(
  bvh_t* bvh, 
  const allocator_t *allocator)
{
  free_bvh_internal(bvh, allocator);
  allocator->mem_free(bvh);
}

////////////////////////////////////////////////////////////////////////////////
// TODO(khalil): cache-align the nodes.
bvh_t*
create_bvh(
  float** vertices, 
  uint32_t** indices, 
  uint32_t* indices_count, 
  uint32_t multi_count,
  const allocator_t* allocator,
  bvh_construction_method_t method)
{
  return 
  bvh_create(vertices, indices, indices_count, multi_count, allocator, method);
}