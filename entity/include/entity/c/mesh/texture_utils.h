/**
 * @file texture_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-12-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_TEXTURE_UTILS_H
#define SCENE_TEXTURE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct texture_t texture_t;
typedef struct allocator_t allocator_t;

ENTITY_API
texture_t* 
allocate_texture_array(
  uint32_t count, 
  const allocator_t* allocator);

ENTITY_API
void
free_texture_array(
  texture_t* texture, 
  uint32_t count,
  const allocator_t* allocator);

ENTITY_API
void
free_texture_internal(
  texture_t* texture, 
  const allocator_t* allocator);

ENTITY_API
void
free_texture(
  texture_t* texture, 
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif