/**
 * @file texture.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <assert.h>
#include <string.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <library/string/cstring.h>
#include <entity/c/mesh/texture.h>


void
texture_def(void *ptr)
{
  assert(ptr);

  {
    texture_t *texture = (texture_t *)ptr;
    memset(texture, 0, sizeof(texture_t));
  }
}

uint32_t 
texture_is_def(const void *ptr)
{
  assert(ptr);

  {
    const texture_t *texture = (const texture_t *)ptr;
    texture_t def; 
    texture_def(&def);
    return texture->path == def.path;
  }
}

void 
texture_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const texture_t *texture = (const texture_t *)src;
    cstring_serialize(texture->path, stream);
  }
}

void 
texture_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    texture_t *texture = (texture_t *)dst;
    texture->path = (cstring_t *)allocator->mem_alloc(sizeof(cstring_t));
    cstring_def(texture->path);
    cstring_deserialize(texture->path, allocator, stream);
  }
}

size_t 
texture_type_size(void)
{
  return sizeof(texture_t);
}

uint32_t 
texture_owns_alloc(void)
{
  return 0;
}

const allocator_t *
texture_get_alloc(const void *ptr)
{
  return NULL;
}

void 
texture_cleanup(
  void *ptr, 
  const allocator_t *allocator)
{
  assert(ptr && !texture_is_def(ptr));
  assert(allocator);

  {
    texture_t *texture = (texture_t *)ptr;
    cstring_free(texture->path, allocator);
  }
}

////////////////////////////////////////////////////////////////////////////////
void
texture_setup(
  texture_t *texture, 
  const char *path, 
  const allocator_t* allocator)
{
  assert(allocator);
  assert(texture && texture_is_def(texture));
  assert(path);

  texture->path = cstring_create(path, allocator);
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_texture_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = texture_def;
  vtable.fn_is_def = texture_is_def;
  vtable.fn_serialize = texture_serialize;
  vtable.fn_deserialize = texture_deserialize;
  vtable.fn_type_size = texture_type_size;
  vtable.fn_owns_alloc = texture_owns_alloc;
  vtable.fn_get_alloc = texture_get_alloc;
  vtable.fn_cleanup = texture_cleanup;
  register_type(get_type_id(texture_t), &vtable);
}