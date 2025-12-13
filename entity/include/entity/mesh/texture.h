/**
 * @file texture.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SCENE_TEXTURE_H
#define SCENE_TEXTURE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/internal/module.h>
#include <library/string/cstring.h>


////////////////////////////////////////////////////////////////////////////////
//| texture_t, '*' = texture
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
// NOTE:
//  - should texture_t own its allocator?
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct texture_t {
  cstring_t path;
} texture_t;

ENTITY_API
void
texture_def(void *ptr);

ENTITY_API
uint32_t
texture_is_def(const void *ptr);

ENTITY_API
void
texture_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
texture_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

ENTITY_API
size_t
texture_type_size(void);

ENTITY_API
uint32_t
texture_owns_alloc(void);

ENTITY_API
const allocator_t *
texture_get_alloc(const void *ptr);

ENTITY_API
void
texture_cleanup(
  void *ptr,
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
texture_setup(
  texture_t *texture,
  const char *path,
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif