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

#include <entity/c/internal/module.h>


typedef struct bvh_t bvh_t;
typedef struct scene_t scene_t;
typedef struct allocator_t allocator_t;

ENTITY_API
bvh_t* 
create_bvh_from_scene(
  scene_t* scene, 
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif