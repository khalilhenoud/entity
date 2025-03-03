/**
 * @file bvh_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-12-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BVH_UTILS_H
#define BVH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <entity/c/spatial/bvh.h>


typedef struct allocator_t allocator_t;

// NOTE: we use multidimensional arrays, to take into account multiple meshes 
// contributing to the bvh.
ENTITY_API
bvh_t*
create_bvh(
  float** vertices, 
  uint32_t** indices, 
  uint32_t* indices_count, 
  uint32_t multi_count,
  const allocator_t* allocator,
  bvh_construction_method_t method);

ENTITY_API
bvh_t* 
allocate_bvh_array(
  uint32_t count, 
  const allocator_t* allocator);

ENTITY_API
void
free_bvh_array(
  bvh_t* bvh, 
  uint32_t count,
  const allocator_t* allocator);

ENTITY_API
void
free_bvh_internal(
  bvh_t* bvh, 
  const allocator_t* allocator);

ENTITY_API
void
free_bvh(
  bvh_t* bvh, 
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif