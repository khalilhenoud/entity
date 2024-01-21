/**
 * @file light_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef SCENE_LIGHT_UTILS_H
#define SCENE_LIGHT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct light_t light_t;
typedef struct allocator_t allocator_t;

ENTITY_API
light_t*
allocate_light_array(
  uint32_t count, 
  const allocator_t* allocator);

ENTITY_API
void
free_light_internal(
  light_t* light, 
  const allocator_t* allocator);

ENTITY_API
void
free_light_array(
  light_t* light, 
  uint32_t count, 
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif