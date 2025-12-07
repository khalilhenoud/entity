/**
 * @file camera.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-09
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <math/c/vector3f.h>


////////////////////////////////////////////////////////////////////////////////
//| camera_t, '*' = camera
//|=============================================================================
//| OPERATION                   | SUPPORTED
//|=============================================================================
//|    *_def                    | YES
//|    *_is_def                 | YES
//|    *_replicate              |
//|    *_fullswap               |
//|    *_serialize              | YES
//|    *_deserialize            | YES
//|    *_hash                   |
//|    *_is_equal               |
//|    *_type_size              | YES
//|    *_type_alignment         |
//|    *_type_id_count          |
//|    *_type_ids               |
//|    *_owns_alloc             |
//|    *_get_alloc              |
//|    *_cleanup                |
////////////////////////////////////////////////////////////////////////////////

typedef struct matrix4f matrix4f;
typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct camera_t {
  vector3f position;
  vector3f lookat_direction;
  vector3f up_vector;
} camera_t;

ENTITY_API
void
camera_def(void *ptr);

ENTITY_API
uint32_t
camera_is_def(const void *ptr);

ENTITY_API
void
camera_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
camera_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

ENTITY_API
size_t
camera_type_size(void);

ENTITY_API
uint32_t
camera_owns_alloc(void);

ENTITY_API
const allocator_t *
camera_get_alloc(const void *ptr);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
camera_setup(
  camera_t *camera,
  const vector3f position,
  const vector3f lookat,
  const vector3f upvector);

ENTITY_API
camera_t*
camera_create(const allocator_t* allocator);

ENTITY_API
void
camera_free(
  camera_t *camera,
  const allocator_t* allocator);

ENTITY_API
void
camera_set_lookat(
  camera_t* camera,
  const vector3f position,
  const vector3f lookat,
  const vector3f upvector);

ENTITY_API
void
camera_view_matrix(
  const camera_t *camera,
  matrix4f *out);

#ifdef __cplusplus
}
#endif

#endif