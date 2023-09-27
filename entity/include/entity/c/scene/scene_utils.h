/**
 * @file scene_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_UTILS_H
#define SCENE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct allocator_t allocator_t;
typedef struct scene_t scene_t;

ENTITY_API
scene_t*
create_scene(
  const char* name, 
  const allocator_t* allocator);

ENTITY_API
void
free_scene(
  scene_t* scene, 
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif