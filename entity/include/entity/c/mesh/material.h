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

#include <entity/c/mesh/color.h>

#define MAX_TEXTURE_COUNT_PER_MATERIAL          8


typedef struct cstring_t cstring_t;

// TODO: We need some utils to create arrays and the likes.
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
  cstring_t* name;
  color_rgba_t ambient;
  color_rgba_t diffuse;
  color_rgba_t specular;
  float shininess;
  float opacity;
  texture_properties_array_t textures;
} material_t;

#endif