/**
 * @file node_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_NODE_UTILS_H
#define SCENE_NODE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct node_t node_t;
typedef struct allocator_t allocator_t;

ENTITY_API
node_t*
allocate_node_array(uint32_t count, const allocator_t* allocator);

ENTITY_API
void
free_node_array(
  node_t* node, 
  uint32_t count, 
  const allocator_t* allocator);

ENTITY_API
void
free_node_internal(node_t* node, const allocator_t* allocator);

ENTITY_API
void
free_node(node_t* node, const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif