/**
 * @file node.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_H
#define SCENE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <math/c/vector3f.h>


////////////////////////////////////////////////////////////////////////////////
//| scene_t, '*' = scene
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
//  - should scene_t own its allocator?
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct cstring_t cstring_t;
typedef struct binary_stream_t binary_stream_t;
typedef struct mesh_t mesh_t;
typedef struct material_t material_t;
typedef struct texture_t texture_t;
typedef struct node_t node_t;
typedef struct font_t font_t;
typedef struct camera_t camera_t;
typedef struct light_t light_t;
typedef struct bvh_t bvh_t;

// TODO: These would be replace with a templated dynamic array (in C).
typedef
struct mesh_repo_t {
  uint32_t count;
  mesh_t *meshes;
} mesh_repo_t;

typedef
struct light_repo_t {
  uint32_t count;
  light_t *lights;
} light_repo_t;

typedef
struct texture_repo_t {
  uint32_t count;
  texture_t *textures;
} texture_repo_t;

typedef
struct material_repo_t {
  uint32_t count;
  material_t *materials;
} material_repo_t;

typedef
struct font_repo_t {
  uint32_t count;
  font_t *fonts;
} font_repo_t;

typedef
struct camera_repo_t {
  uint32_t count;
  camera_t *cameras;
} camera_repo_t;

typedef
struct node_repo_t {
  uint32_t count;
  node_t *nodes;
} node_repo_t;

typedef
struct bvh_repo_t {
  uint32_t count;
  bvh_t *bvhs;
} bvh_repo_t;

typedef
struct scene_metadata_t {
  point3f player_start;
  float player_angle;
} scene_metadata_t;

// TODO: assimp supports a camera/lights/skeletons/animations repo which we 
// should support here.
typedef
struct scene_t {
  cstring_t *name;
  scene_metadata_t metadata;
  node_repo_t node_repo;        // root is: node_repo.nodes[0];
  light_repo_t light_repo;
  mesh_repo_t mesh_repo;
  material_repo_t material_repo;
  texture_repo_t texture_repo;
  font_repo_t font_repo;
  camera_repo_t camera_repo;
  bvh_repo_t bvh_repo;
} scene_t;

ENTITY_API
void
scene_def(void *ptr);

ENTITY_API
uint32_t 
scene_is_def(const void *ptr);

ENTITY_API
void 
scene_serialize(
  const void *src, 
  binary_stream_t *stream);

ENTITY_API
void 
scene_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t* stream);

ENTITY_API
size_t 
scene_type_size(void);

ENTITY_API
uint32_t 
scene_owns_alloc(void);

ENTITY_API
const allocator_t *
scene_get_alloc(const void *ptr);

ENTITY_API
void 
scene_cleanup(
  void *ptr, 
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
scene_setup(
  scene_t *scene, 
  const char *name,
  const allocator_t* allocator);

ENTITY_API
scene_t*
scene_create(
  const char* name, 
  const allocator_t* allocator);

ENTITY_API
void
scene_free(
  scene_t* scene, 
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif