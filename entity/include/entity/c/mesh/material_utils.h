/**
 * @file material_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-12-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_MATERIAL_UTILS_H
#define SCENE_MATERIAL_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct material_t material_t;
typedef struct allocator_t allocator_t;

ENTITY_API
material_t* 
allocate_material_array(
  uint32_t count, 
  const allocator_t* allocator);

ENTITY_API
void
free_material_array(
  material_t* material, 
  uint32_t count,
  const allocator_t* allocator);

ENTITY_API
void
free_material_internal(
  material_t* material, 
  const allocator_t* allocator);

ENTITY_API
void
free_material(
  material_t* material, 
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif