/**
 * @file animation.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-01-07
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <string.h>
#include <entity/scene/animation.h>
#include <library/allocator/allocator.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
animation_def(void *ptr)
{
  assert(ptr);

  {
    animation_t *animation = (animation_t *)ptr;
    memset(animation, 0, sizeof(animation_t));
  }
}

uint32_t
animation_is_def(const void *ptr)
{
  assert(ptr);

  {
    const animation_t *animation = (const animation_t *)ptr;
    animation_t def;
    animation_def(&def);
    return !memcmp(animation, &def, sizeof(animation_t));
  }
}

void
animation_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const animation_t *animation = (const animation_t *)src;
    cstring_serialize(&animation->name, stream);
    binary_stream_write(stream, &animation->duration, sizeof(float));
    binary_stream_write(stream, &animation->ticks_per_second, sizeof(float));
    cvector_serialize(&animation->channels, stream);
  }
}

void
animation_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    animation_t *animation = (animation_t *)dst;
    cstring_def(&animation->name);
    cstring_deserialize(&animation->name, allocator, stream);
    binary_stream_read(
      stream, (uint8_t *)&animation->duration,
      sizeof(float), sizeof(float));
    binary_stream_read(
      stream, (uint8_t *)&animation->ticks_per_second,
      sizeof(float), sizeof(float));
    cvector_def(&animation->channels);
    cvector_deserialize(&animation->channels, allocator, stream);
  }
}

size_t
animation_type_size(void)
{
  return sizeof(animation_t);
}

uint32_t
animation_owns_alloc(void)
{
  return 0;
}

const allocator_t *
animation_get_alloc(const void *ptr)
{
  return NULL;
}

void
animation_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !animation_is_def(ptr));
  assert(allocator);

  {
    animation_t *animation = (animation_t *)ptr;
    cstring_cleanup2(&animation->name);
    cvector_cleanup2(&animation->channels);
    animation_def(animation);
  }
}

void
anim_node_def(void *ptr)
{
  assert(ptr);

  {
    anim_node_t *anim_node = (anim_node_t *)ptr;
    memset(anim_node, 0, sizeof(anim_node_t));
  }
}

uint32_t
anim_node_is_def(const void *ptr)
{
  assert(ptr);

  {
    const anim_node_t *anim_node = (const anim_node_t *)ptr;
    anim_node_t def;
    anim_node_def(&def);
    return !memcmp(anim_node, &def, sizeof(anim_node_t));
  }
}

void
anim_node_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const anim_node_t *anim_node = (const anim_node_t *)src;
    cstring_serialize(&anim_node->name, stream);
    cvector_serialize(&anim_node->position_keys, stream);
    cvector_serialize(&anim_node->rotation_keys, stream);
    cvector_serialize(&anim_node->scale_keys, stream);
  }
}

void
anim_node_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    anim_node_t *anim_node = (anim_node_t *)dst;
    cstring_def(&anim_node->name);
    cstring_deserialize(&anim_node->name, allocator, stream);
    cvector_def(&anim_node->position_keys);
    cvector_deserialize(&anim_node->position_keys, allocator, stream);
    cvector_def(&anim_node->rotation_keys);
    cvector_deserialize(&anim_node->rotation_keys, allocator, stream);
    cvector_def(&anim_node->scale_keys);
    cvector_deserialize(&anim_node->scale_keys, allocator, stream);
  }
}

size_t
anim_node_type_size(void)
{
  return sizeof(anim_node_t);
}

uint32_t
anim_node_owns_alloc(void)
{
  return 0;
}

const allocator_t *
anim_node_get_alloc(const void *ptr)
{
  return NULL;
}

void
anim_node_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !anim_node_is_def(ptr));
  assert(allocator);

  {
    anim_node_t *anim_node = (anim_node_t *)ptr;
    cstring_cleanup2(&anim_node->name);
    cvector_cleanup2(&anim_node->position_keys);
    cvector_cleanup2(&anim_node->rotation_keys);
    cvector_cleanup2(&anim_node->scale_keys);
    anim_node_def(anim_node);
  }
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_animation_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = animation_def;
  vtable.fn_is_def = animation_is_def;
  vtable.fn_serialize = animation_serialize;
  vtable.fn_deserialize = animation_deserialize;
  vtable.fn_type_size = animation_type_size;
  vtable.fn_owns_alloc = animation_owns_alloc;
  vtable.fn_get_alloc = animation_get_alloc;
  vtable.fn_cleanup = animation_cleanup;
  register_type(get_type_id(animation_t), &vtable);
}

INITIALIZER(register_anim_node_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = anim_node_def;
  vtable.fn_is_def = anim_node_is_def;
  vtable.fn_serialize = anim_node_serialize;
  vtable.fn_deserialize = anim_node_deserialize;
  vtable.fn_type_size = anim_node_type_size;
  vtable.fn_owns_alloc = anim_node_owns_alloc;
  vtable.fn_get_alloc = anim_node_get_alloc;
  vtable.fn_cleanup = anim_node_cleanup;
  register_type(get_type_id(anim_node_t), &vtable);
}