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
#include <math/c/matrix4f.h>


typedef struct string_t string_t;
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
  light_t* lights;
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
  bvh_t* bvhs;
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
  string_t* name;
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

#endif