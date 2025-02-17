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

#include <math/c/vector3f.h>
#include <entity/c/mesh/color.h>


typedef struct cstring_t cstring_t;

typedef
enum light_type_t {
  LIGHT_TYPE_POINT,
  LIGHT_TYPE_SPOT,
  LIGHT_TYPE_DIRECTIONAL,
  LIGHT_TYPE_COUNT
} light_type_t;

// NOTE: lights are relative to their scene space.
// NOTE: direction/up/cone not applicable to point lights.
// NOTE: cones are not applicable to point/directional lights.
// NOTE: Attenuation is not applicable to directional lights.
// final_attenuation = 1/(constant_att + linear_att * d + quad_att * d*d).
typedef
struct light_t {
  cstring_t* name;
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

#endif