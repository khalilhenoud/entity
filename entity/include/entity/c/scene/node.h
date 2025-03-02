/**
 * @file node.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <math/c/matrix4f.h>


////////////////////////////////////////////////////////////////////////////////
//| node_t, '*' = node
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
//  - should node_t own its allocator?
//  - node_t has not node_setup() function.
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct cstring_t cstring_t;
typedef struct binary_stream_t binary_stream_t;

// Indicies into the scene payloads.
typedef
struct payload_indices_t {
  uint32_t count;
  uint32_t *indices;
} payload_indices_t;

typedef
struct node_t {
  cstring_t *name;
  matrix4f transform;
  payload_indices_t meshes;
  payload_indices_t nodes;
} node_t;

ENTITY_API
void
node_def(void *ptr);

ENTITY_API
uint32_t 
node_is_def(const void *ptr);

ENTITY_API
void 
node_serialize(
  const void *src, 
  binary_stream_t *stream);

ENTITY_API
void 
node_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t* stream);

ENTITY_API
size_t 
node_type_size(void);

ENTITY_API
uint32_t 
node_owns_alloc(void);

ENTITY_API
const allocator_t *
node_get_alloc(const void *ptr);

ENTITY_API
void 
node_cleanup(
  void *ptr, 
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif