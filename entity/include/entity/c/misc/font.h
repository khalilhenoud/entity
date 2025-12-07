/**
 * @file font.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2023-09-28
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef MISC_FONT_H
#define MISC_FONT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <entity/c/internal/module.h>
#include <library/string/cstring.h>


////////////////////////////////////////////////////////////////////////////////
//| font_t, '*' = font
//|=============================================================================
//| OPERATION                   | SUPPORTED
//|=============================================================================
//|    *_def                    | YES
//|    *_is_def                 | YES
//|    *_replicate              |
//|    *_fullswap               |
//|    *_serialize              | YES
//|    *_deserialize            | YES
//|    *_hash                   |
//|    *_is_equal               |
//|    *_type_size              | YES
//|    *_type_alignment         |
//|    *_type_id_count          |
//|    *_type_ids               |
//|    *_owns_alloc             | YES
//|    *_get_alloc              | YES
//|    *_cleanup                | YES
////////////////////////////////////////////////////////////////////////////////
// NOTE:
//  - should font_t own its allocator?
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct font_t {
  cstring_t image_file;
  cstring_t data_file;
} font_t;

ENTITY_API
void
font_def(void *ptr);

ENTITY_API
uint32_t
font_is_def(const void *ptr);

ENTITY_API
void
font_serialize(
  const void *src,
  binary_stream_t *stream);

ENTITY_API
void
font_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

ENTITY_API
size_t
font_type_size(void);

ENTITY_API
uint32_t
font_owns_alloc(void);

ENTITY_API
const allocator_t *
font_get_alloc(const void *ptr);

ENTITY_API
void
font_cleanup(
  void *ptr,
  const allocator_t* allocator);

////////////////////////////////////////////////////////////////////////////////
ENTITY_API
void
font_setup(
  font_t *font,
  const char *image_file,
  const char *data_file,
  const allocator_t *allocator);

ENTITY_API
font_t*
font_create(
  const char *image_file,
  const char *data_file,
  const allocator_t *allocator);

ENTITY_API
void
font_free(
  font_t *font,
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif