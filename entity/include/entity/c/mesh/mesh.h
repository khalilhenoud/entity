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

#define MAX_MATERIAL_NUMBER           4


typedef
struct material_array_t {
  uint32_t used;
  uint32_t indices[MAX_MATERIAL_NUMBER];
} material_array_t;

typedef
struct mesh_t {
  float* vertices;
  float* normals;
  float* uvs;
  uint32_t vertices_count;
  uint32_t* indices;
  uint32_t indices_count;
  material_array_t materials;
} mesh_t;

#endif