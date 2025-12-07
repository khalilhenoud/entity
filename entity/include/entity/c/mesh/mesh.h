/**
 * @file mesh.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef SCENE_MESH_H
#define SCENE_MESH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <library/containers/cvector.h>

#define MAX_MATERIAL_NUMBER           4


////////////////////////////////////////////////////////////////////////////////
//| mesh_t, '*' = mesh
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
//  - should mesh_t own its allocator?
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct material_array_t {
  uint32_t used;
  uint32_t indices[MAX_MATERIAL_NUMBER];
} material_array_t;

typedef
struct mesh_t {
  cvector_t vertices;           // float
  cvector_t normals;            // float
  cvector_t uvs;                // float
  cvector_t indices;            // uint32_t
  material_array_t materials;
} mesh_t;

ENTITY_API
void
mesh_def(void *ptr);

ENTITY_API
uint32_t
mesh_is_def(const void *ptr);

ENTITY_API
void
mesh_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
mesh_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream);

ENTITY_API
size_t
mesh_type_size(void);

ENTITY_API
uint32_t
mesh_owns_alloc(void);

ENTITY_API
const allocator_t *
mesh_get_alloc(const void *ptr);

ENTITY_API
void
mesh_cleanup(
  void *ptr,
  const allocator_t *allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
mesh_setup(
  mesh_t *mesh,
  float *vertices,
  float *normals,
  float *uvs,
  uint32_t vertices_count,
  uint32_t *indices,
  uint32_t indices_count,
  material_array_t materials,
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif