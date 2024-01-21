/**
 * @file camera_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_CAMERA_UTILS_H
#define SCENE_CAMERA_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math/c/vector3f.h>
#include <entity/c/internal/module.h>


typedef struct camera_t camera_t;
typedef struct allocator_t allocator_t;
typedef struct matrix4f matrix4f;

// NOTE: Since camera_t has no internal dynamic data, array functionality is not
// required.
ENTITY_API
camera_t*
create_camera(const allocator_t* allocator);

ENTITY_API
void
free_camera(camera_t* camera, const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
set_camera_lookat(
  camera_t* camera, 
  const vector3f position, 
  const vector3f lookat, 
  const vector3f upvector);

ENTITY_API
void
get_view_transformation(
  const camera_t* camera, 
  matrix4f* out);

#ifdef __cplusplus
}
#endif

#endif