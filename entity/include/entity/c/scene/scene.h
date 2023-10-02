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

#include <stdint.h>
#include <library/string/fixed_string.h>
#include <math/c/matrix4f.h>


typedef struct mesh_t mesh_t;
typedef struct material_t material_t;
typedef struct texture_t texture_t;
typedef struct node_t node_t;
typedef struct font_t font_t;
typedef struct camera_t camera_t;

// TODO: These would be replace with a templated dynamic array (in C).
typedef
struct mesh_repo_t {
  uint32_t count;
  mesh_t *meshes;
} mesh_repo_t;

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

// TODO: assimp supports a camera/lights/skeletons/animations repo which we 
// should support here.
typedef
struct scene_t {
  fixed_str_128_t name;
  node_repo_t node_repo;        // root is: node_repo.nodes[0];
  mesh_repo_t mesh_repo;
  material_repo_t material_repo;
  texture_repo_t texture_repo;
  font_repo_t font_repo;
  camera_repo_t camera_repo;  
} scene_t;

#endif