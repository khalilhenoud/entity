/**
 * @file material.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_MATERIAL_H
#define SCENE_MATERIAL_H

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <entity/c/mesh/color.h>

#define MAX_TEXTURE_COUNT_PER_MATERIAL          8


////////////////////////////////////////////////////////////////////////////////
//| material_t, '*' = material
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
//  - should material_t own its allocator?
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct cstring_t cstring_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct texture_properties_t {
  uint32_t index;
  float u, v;
  float u_scale, v_scale;
  float angle;
} texture_properties_t;

typedef
struct texture_properties_array_t {
  uint32_t used;
  texture_properties_t data[MAX_TEXTURE_COUNT_PER_MATERIAL];
} texture_properties_array_t;

typedef
struct material_t {
  cstring_t *name;
  color_rgba_t ambient;
  color_rgba_t diffuse;
  color_rgba_t specular;
  float shininess;
  float opacity;
  texture_properties_array_t textures;
} material_t;

ENTITY_API
void
material_def(void *ptr);

ENTITY_API
uint32_t 
material_is_def(const void *ptr);

ENTITY_API
void 
material_serialize(
  const void *src, 
  binary_stream_t *stream);

ENTITY_API
void 
material_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t* stream);

ENTITY_API
size_t 
material_type_size(void);

ENTITY_API
uint32_t 
material_owns_alloc(void);

ENTITY_API
const allocator_t *
material_get_alloc(const void *ptr);

ENTITY_API
void 
material_cleanup(
  void *ptr, 
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
material_setup(
  material_t *material, 
  const char *name, 
  color_rgba_t ambient,
  color_rgba_t diffuse,
  color_rgba_t specular,
  float shininess,
  float opacity,
  texture_properties_array_t textures,
  const allocator_t* allocator);

#endif