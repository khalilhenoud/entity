/**
 * @file skinned_mesh.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2025-12-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <assert.h>
#include <string.h>
#include <entity/mesh/skinned_mesh.h>
#include <library/allocator/allocator.h>
#include <library/core/core.h>
#include <library/streams/binary_stream.h>
#include <library/type_registry/type_registry.h>


void
skinned_mesh_def(void *ptr)
{
  assert(ptr);

  {
    skinned_mesh_t *skinned_mesh = (skinned_mesh_t *)ptr;
    memset(skinned_mesh, 0, sizeof(skinned_mesh_t));
  }
}

uint32_t
skinned_mesh_is_def(const void *ptr)
{
  assert(ptr);

  {
    const skinned_mesh_t *skinned_mesh = (const skinned_mesh_t *)ptr;
    skinned_mesh_t def;
    skinned_mesh_def(&def);
    return !memcmp(skinned_mesh, &def, sizeof(skinned_mesh_t));
  }
}

void
skinned_mesh_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const skinned_mesh_t *skinned_mesh = (const skinned_mesh_t *)src;
    mesh_serialize(&skinned_mesh->mesh, stream);
  }
}

void
skinned_mesh_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    skinned_mesh_t *skinned_mesh = (skinned_mesh_t *)dst;
    mesh_deserialize(&skinned_mesh->mesh, allocator, stream);
  }
}

size_t
skinned_mesh_type_size(void)
{
  return sizeof(skinned_mesh_t);
}

uint32_t
skinned_mesh_owns_alloc(void)
{
  return 0;
}

const allocator_t *
skinned_mesh_get_alloc(const void *ptr)
{
  return NULL;
}

void
skinned_mesh_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !skinned_mesh_is_def(ptr));
  assert(allocator);

  {
    skinned_mesh_t *skinned_mesh = (skinned_mesh_t *)ptr;
    mesh_cleanup(&skinned_mesh->mesh, allocator);
    memset(skinned_mesh, 0, sizeof(skinned_mesh_t));
  }
}

////////////////////////////////////////////////////////////////////////////////
void
skinned_mesh_setup(
  skinned_mesh_t *skinned_mesh,
  float *vertices,
  float *normals,
  float *uvs,
  uint32_t vertices_count,
  uint32_t *indices,
  uint32_t indices_count,
  material_array_t materials,
  const allocator_t *allocator)
{
  assert(allocator);
  assert(skinned_mesh && skinned_mesh_is_def(skinned_mesh));
  assert(vertices && normals && uvs && indices);

  mesh_setup(
    &skinned_mesh->mesh,
    vertices,
    normals,
    uvs,
    vertices_count,
    indices,
    indices_count,
    materials,
    allocator);
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_skinned_mesh_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = skinned_mesh_def;
  vtable.fn_is_def = skinned_mesh_is_def;
  vtable.fn_serialize = skinned_mesh_serialize;
  vtable.fn_deserialize = skinned_mesh_deserialize;
  vtable.fn_type_size = skinned_mesh_type_size;
  vtable.fn_owns_alloc = skinned_mesh_owns_alloc;
  vtable.fn_get_alloc = skinned_mesh_get_alloc;
  vtable.fn_cleanup = skinned_mesh_cleanup;
  register_type(get_type_id(skinned_mesh_t), &vtable);
}