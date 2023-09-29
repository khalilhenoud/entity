/**
 * @file font_utils.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MISC_FONT_UTILS_H
#define MISC_FONT_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>


typedef struct allocator_t allocator_t;
typedef struct font_t font_t;

ENTITY_API
font_t*
create_font(
  const char *image_file, 
  const char *data_file, 
  const allocator_t* allocator);

ENTITY_API
void
free_font(
  font_t* font, 
  const allocator_t* allocator);

#ifdef __cplusplus
}
#endif

#endif