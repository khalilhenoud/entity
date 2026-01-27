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
#include <library/containers/cvector.h>
#include <library/string/cstring.h>
#include <math/matrix4f.h>


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

////////////////////////////////////////////////////////////////////////////////
//| skel_node_t, '*' = skel_node
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
//| bone_t, '*' = bone
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
struct skel_node_t {
  cstring_t name;               // corresponds to a bone, unless it is a helper
  matrix4f transform;
  cvector_t skel_nodes;         // uint32_t, index into skeleton_t
  uint32_t bone_index;          // uint32_t max means it is inalid
} skel_node_t;

// NOTE: due to simplicity, I am not currently not treating this as a separate
// type.
typedef
struct skeleton_t {
  cvector_t nodes;              // skel_node_t
} skeleton_t;

typedef
struct vertex_weight_t {
  uint32_t vertex_id;
  float weight;
} vertex_weight_t;

typedef
struct bone_t {
  cstring_t name;
  matrix4f offset_matrix;       // transform vertex into the bone local space
  cvector_t vertex_weights;
} bone_t;

typedef
struct skinned_mesh_t {
  mesh_t mesh;
  cvector_t bones;
  skeleton_t skeleton;
} skinned_mesh_t;

ENTITY_API
void
skel_node_def(void *ptr);

ENTITY_API
void
bone_def(void *ptr);

ENTITY_API
void
skinned_mesh_def(void *ptr);

ENTITY_API
uint32_t
skel_node_is_def(const void *ptr);

ENTITY_API
uint32_t
bone_is_def(const void *ptr);

ENTITY_API
uint32_t
skinned_mesh_is_def(const void *ptr);

ENTITY_API
void
skel_node_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
bone_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
skinned_mesh_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
skel_node_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
void
bone_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
void
skinned_mesh_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
size_t
skel_node_type_size(void);

ENTITY_API
size_t
bone_type_size(void);

ENTITY_API
size_t
skinned_mesh_type_size(void);

ENTITY_API
uint32_t
skel_node_owns_alloc(void);

ENTITY_API
uint32_t
bone_owns_alloc(void);

ENTITY_API
uint32_t
skinned_mesh_owns_alloc(void);

ENTITY_API
const allocator_t *
skel_node_get_alloc(const void *ptr);

ENTITY_API
const allocator_t *
bone_get_alloc(const void *ptr);

ENTITY_API
const allocator_t *
skinned_mesh_get_alloc(const void *ptr);

ENTITY_API
void
skel_node_cleanup(
  void *ptr,
  const allocator_t *allocator);

ENTITY_API
void
bone_cleanup(
  void *ptr,
  const allocator_t *allocator);

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