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


typedef struct face_t face_t;

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

#ifdef __cplusplus
}
#endif

#endif