/**
 * @file skinned_mesh.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2025-12-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef SCENE_SKINNED_MESH_H
#define SCENE_SKINNED_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/internal/module.h>
#include <entity/mesh/mesh.h>


////////////////////////////////////////////////////////////////////////////////
//| skinned_mesh_t, '*' = skinned_mesh
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
struct skinned_mesh_t {
  mesh_t mesh;
} skinned_mesh_t;

ENTITY_API
void
skinned_mesh_def(void *ptr);

ENTITY_API
uint32_t
skinned_mesh_is_def(const void *ptr);

ENTITY_API
void
skinned_mesh_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
skinned_mesh_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
size_t
skinned_mesh_type_size(void);

ENTITY_API
uint32_t
skinned_mesh_owns_alloc(void);

ENTITY_API
const allocator_t *
skinned_mesh_get_alloc(const void *ptr);

ENTITY_API
void
skinned_mesh_cleanup(
  void *ptr,
  const allocator_t *allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
skinned_mesh_setup(
  skinned_mesh_t *skinned_mesh,
  float *vertices,
  float *normals,
  float *uvs,
  uint32_t vertices_count,
  uint32_t *indices,
  uint32_t indices_count,
  material_array_t materials,
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif