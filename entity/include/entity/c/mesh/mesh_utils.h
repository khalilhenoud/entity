/**
 * @file mesh_utils.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SCENE_MESH_UTILS_H
#define SCENE_MESH_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct mesh_t mesh_t;
typedef struct allocator_t allocator_t;

ENTITY_API
mesh_t*
create_unit_cube(const allocator_t *allocator);

ENTITY_API
mesh_t*
create_unit_sphere(const int32_t factor, const allocator_t *allocator);

ENTITY_API
mesh_t*
create_unit_capsule(
  const int32_t factor,
  const float half_height_to_radius_ratio,
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif