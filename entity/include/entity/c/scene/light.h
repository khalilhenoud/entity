/**
 * @file light.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SCENE_LIGHT_H
#define SCENE_LIGHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <entity/c/mesh/color.h>
#include <library/string/cstring.h>
#include <math/c/vector3f.h>


////////////////////////////////////////////////////////////////////////////////
//| light_t, '*' = light
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
//  - should light_t own its allocator?
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
enum light_type_t {
  LIGHT_TYPE_POINT,
  LIGHT_TYPE_SPOT,
  LIGHT_TYPE_DIRECTIONAL,
  LIGHT_TYPE_COUNT
} light_type_t;

// NOTE: 
//  - lights are relative to their scene space.
//  - direction/up/cone not applicable to point lights.
//  - cones are not applicable to point/directional lights.
//  - Attenuation is not applicable to directional lights.
//  - final_attenuation = 1/(constant_att + linear_att * d + quad_att * d*d).
typedef
struct light_t {
  cstring_t name;
  vector3f position;
  vector3f direction;
  vector3f up;
  float inner_cone;
  float outer_cone;
  float attenuation_constant;
  float attenuation_linear;
  float attenuation_quadratic;
  color_rgba_t diffuse;
  color_rgba_t specular;
  color_rgba_t ambient;
  light_type_t type;
} light_t;

ENTITY_API
void
light_def(void *ptr);

ENTITY_API
uint32_t 
light_is_def(const void *ptr);

ENTITY_API
void 
light_serialize(
  const void *src, 
  binary_stream_t *stream);

ENTITY_API
void 
light_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t* stream);

ENTITY_API
size_t 
light_type_size(void);

ENTITY_API
uint32_t 
light_owns_alloc(void);

ENTITY_API
const allocator_t *
light_get_alloc(const void *ptr);

ENTITY_API
void 
light_cleanup(
  void *ptr, 
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
light_setup(
  light_t *light, 
  const char *name, 
  vector3f position,
  vector3f direction,
  vector3f up,
  float inner_cone,
  float outer_cone,
  float attenuation_constant,
  float attenuation_linear,
  float attenuation_quadratic,
  color_rgba_t diffuse,
  color_rgba_t specular,
  color_rgba_t ambient,
  light_type_t type,
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif