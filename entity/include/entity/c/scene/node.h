/**
 * @file node.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <stdint.h>
#include <math/c/matrix4f.h>


typedef struct string_t string_t;

// Indicies into the scene payloads.
typedef
struct payload_indices_t {
  uint32_t count;
  uint32_t *indices;
} payload_indices_t;

typedef
struct node_t {
  string_t* name;
  matrix4f transform;
  payload_indices_t meshes;
  payload_indices_t nodes;
} node_t;

#endif