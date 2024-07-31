/**
 * @file bvh.h
 * @author khalilhenoud@gmail.com
 * @brief non-general bvh for simplistic collision detection.
 * @version 0.1
 * @date 2023-10-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SPATIAL_BVH_H
#define SPATIAL_BVH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <math/c/vector3f.h>


typedef struct allocator_t allocator_t;
typedef struct face_t face_t;

// TODO(khalil): cache-align the nodes.

typedef
struct bvh_aabb_t {
  point3f min_max[2];
} bvh_aabb_t;

// NOTE: left_first meaning depends on the tri_count. If tri_count is 0, the 
// node is an interior node and left_first will be an offset into the node 
// array, otherwise we are dealing with a leaf and left_first will be an offset
// into the triangle array.
typedef
struct bvh_node_t {
  bvh_aabb_t bounds;
  uint32_t left_first, tri_count;
} bvh_node_t;

// NOTE: the max size of node array is max: 2 * (count + 1) - 1.
// We also allocate an extra unused node that is left unused so we can adhere to
// cache alignment in the future.
typedef
struct bvh_t {
  face_t* faces;
  vector3f* normals;
  bvh_aabb_t* bounds;
  uint32_t count;
  bvh_node_t* nodes;
  uint32_t nodes_used;
} bvh_t;

typedef
enum bvh_construction_method_t {
  BVH_CONSTRUCT_NAIVE,
  BVH_CONSTRUCT_COUNT
} bvh_construction_method_t;

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
void
free_bvh(bvh_t* bvh, const allocator_t* allocator);

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