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


typedef struct bvh_t bvh_t;
typedef struct bvh_aabb_t bvh_aabb_t;
typedef struct allocator_t allocator_t;

typedef
enum bvh_construction_method_t {
  BVH_CONSTRUCT_NAIVE,
  BVH_CONSTRUCT_COUNT
} bvh_construction_method_t;

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

////////////////////////////////////////////////////////////////////////////////

inline
uint32_t
get_max_node_count(uint32_t count)
{
  return 2 * (count + 1) - 1; 
}

ENTITY_API
void
merge_aabb(
  bvh_aabb_t* dst, 
  const bvh_aabb_t* a, 
  const bvh_aabb_t* b);

ENTITY_API
void
merge_aabb_inplace(
  bvh_aabb_t* dst, 
  const bvh_aabb_t* b);

ENTITY_API
uint32_t
get_bvh_primitives_per_leaf(void);

ENTITY_API
int32_t
bounds_intersect(const bvh_aabb_t* left, const bvh_aabb_t* right);

// NOTE: A max 256 leaf indices returned, if used is 0 no intersection.
ENTITY_API
void
query_intersection_fixed_256(
  bvh_t* bvh, 
  bvh_aabb_t* bound, 
  uint32_t array[256], 
  uint32_t* used);

#ifdef __cplusplus
}
#endif

#endif