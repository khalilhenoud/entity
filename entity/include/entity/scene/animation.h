/**
 * @file animation.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-01-07
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef SCENE_ANIMATION_H
#define SCENE_ANIMATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/internal/module.h>
#include <library/containers/cvector.h>
#include <library/string/cstring.h>
#include <math/quatf.h>
#include <math/vector3f.h>


////////////////////////////////////////////////////////////////////////////////
//| animation_t, '*' = animation
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
//|    *_owns_alloc             | YES
//|    *_get_alloc              | YES
//|    *_cleanup                | YES
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//| anim_node_t, '*' = anim_node
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
//|    *_owns_alloc             | YES
//|    *_get_alloc              | YES
//|    *_cleanup                | YES
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct position_key_t {
  float time;
  vector3f value;
} position_key_t;

typedef
struct rotation_key_t {
  float time;
  quatf value;
} rotation_key_t;

typedef
struct scale_key_t {
  float time;
  vector3f value;
} scale_key_t;

typedef
struct anim_node_t {
  cstring_t name;
  cvector_t position_keys;
  cvector_t rotation_keys;
  cvector_t scale_keys;
} anim_node_t;

typedef
struct animation_t {
  cstring_t name;
  float duration;
  float ticks_per_second;
  cvector_t channels;                 // anim_node_t
} animation_t;

ENTITY_API
void
animation_def(void *ptr);

ENTITY_API
uint32_t
animation_is_def(const void *ptr);

ENTITY_API
void
animation_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
animation_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
size_t
animation_type_size(void);

ENTITY_API
uint32_t
animation_owns_alloc(void);

ENTITY_API
const allocator_t *
animation_get_alloc(const void *ptr);

ENTITY_API
void
animation_cleanup(
  void *ptr,
  const allocator_t *allocator);

ENTITY_API
void
anim_node_def(void *ptr);

ENTITY_API
uint32_t
anim_node_is_def(const void *ptr);

ENTITY_API
void
anim_node_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
anim_node_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
size_t
anim_node_type_size(void);

ENTITY_API
uint32_t
anim_node_owns_alloc(void);

ENTITY_API
const allocator_t *
anim_node_get_alloc(const void *ptr);

ENTITY_API
void
anim_node_cleanup(
  void *ptr,
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif