/**
 * @file image_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef RUNTIME_IMAGE_UTILS_H
#define RUNTIME_IMAGE_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct texture_t texture_t;
typedef struct texture_runtime_t texture_runtime_t;
typedef texture_runtime_t image_t;
typedef struct allocator_t allocator_t;

ENTITY_API
texture_runtime_t*
create_texture_runtime(
  const texture_t* texture, 
  const allocator_t* allocator);

ENTITY_API
void
free_texture_runtime(
  texture_runtime_t* texture_runtime, 
  const allocator_t* allocator);

ENTITY_API
void
free_texture_runtime_internal(
  texture_runtime_t* texture_runtime, 
  const allocator_t* allocator);

ENTITY_API
void
allocate_runtime_buffer(
  texture_runtime_t* texture_runtime, 
  const size_t buffer_size, 
  const allocator_t* allocator);

ENTITY_API
void
free_runtime_buffer(
  texture_runtime_t* texture_runtime, 
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
uint32_t
get_image_component_count(const image_t* image);

ENTITY_API
void
get_image_extension(const image_t* image, uint8_t extension[8]);

#ifdef __cplusplus
}
#endif

#endif