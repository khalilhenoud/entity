/**
 * @file node.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <assert.h>
#include <string.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <entity/c/scene/node.h>


void
node_def(void *ptr)
{
  assert(ptr);

  {
    node_t *node = (node_t *)ptr;
    memset(node, 0, sizeof(node_t));
  }
}

uint32_t 
node_is_def(const void *ptr)
{
  assert(ptr);

  {
    const node_t *node = (const node_t *)ptr;
    node_t def; 
    node_def(&def);
    return !memcmp(node, &def, sizeof(node_t));
  }
}

void 
node_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const node_t *node = (const node_t *)src;
    cstring_serialize(&node->name, stream);
    binary_stream_write(stream, &node->transform, sizeof(matrix4f));
    cvector_serialize(&node->meshes, stream);
    cvector_serialize(&node->nodes, stream);
  }
}

void 
node_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    uint32_t i = 0;
    node_t *node = (node_t *)dst;
    cstring_def(&node->name);
    cstring_deserialize(&node->name, allocator, stream);
    binary_stream_read(
      stream, (uint8_t *)&node->transform, sizeof(matrix4f), sizeof(matrix4f));
    cvector_setup(&node->meshes, get_type_data(uint32_t), 0, allocator);
    cvector_deserialize(&node->meshes, allocator, stream);
    cvector_setup(&node->nodes, get_type_data(uint32_t), 0, allocator);
    cvector_deserialize(&node->nodes, allocator, stream);
  }
}

size_t 
node_type_size(void)
{
  return sizeof(node_t);
}

uint32_t 
node_owns_alloc(void)
{
  return 0;
}

const allocator_t *
node_get_alloc(const void *ptr)
{
  return NULL;
}

void 
node_cleanup(
  void *ptr, 
  const allocator_t *allocator)
{
  assert(ptr && !node_is_def(ptr));
  assert(allocator);

  {
    node_t *node = (node_t *)ptr;
    cstring_cleanup2(&node->name);
    cvector_cleanup2(&node->meshes);
    cvector_cleanup2(&node->nodes);
    node_def(node);
  }
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_node_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = node_def;
  vtable.fn_is_def = node_is_def;
  vtable.fn_serialize = node_serialize;
  vtable.fn_deserialize = node_deserialize;
  vtable.fn_type_size = node_type_size;
  vtable.fn_owns_alloc = node_owns_alloc;
  vtable.fn_get_alloc = node_get_alloc;
  vtable.fn_cleanup = node_cleanup;
  register_type(get_type_id(node_t), &vtable);
}