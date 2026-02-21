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
skel_node_def(void *ptr)
{
  assert(ptr);

  {
    skel_node_t *node = (skel_node_t *)ptr;
    memset(node, 0, sizeof(skel_node_t));
  }
}

void
bone_def(void *ptr)
{
  assert(ptr);

  {
    bone_t *bone = (bone_t *)ptr;
    memset(bone, 0, sizeof(bone_t));
  }
}

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
skel_node_is_def(const void *ptr)
{
  assert(ptr);

  {
    const skel_node_t *node = (const skel_node_t *)ptr;
    skel_node_t def;
    skel_node_def(&def);
    return !memcmp(node, &def, sizeof(skel_node_t));
  }
}

uint32_t
bone_is_def(const void *ptr)
{
  assert(ptr);

  {
    const bone_t *bone = (const bone_t *)ptr;
    bone_t def;
    bone_def(&def);
    return !memcmp(bone, &def, sizeof(bone_t));
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
skel_node_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const skel_node_t *node = (const skel_node_t *)src;
    cstring_serialize(&node->name, stream);
    binary_stream_write(stream, &node->transform, sizeof(matrix4f));
    binary_stream_write(stream, &node->bone_index, sizeof(uint32_t));
    cvector_serialize(&node->skel_nodes, stream);
  }
}

void
bone_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const bone_t *bone = (const bone_t *)src;
    cstring_serialize(&bone->name, stream);
    binary_stream_write(stream, &bone->offset_matrix, sizeof(matrix4f));
    cvector_serialize(&bone->vertex_weights, stream);
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
    cvector_serialize(&skinned_mesh->bones, stream);
    cvector_serialize(&skinned_mesh->skeleton.nodes, stream);
  }
}

void
skel_node_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    skel_node_t *node = (skel_node_t *)dst;
    cstring_def(&node->name);
    cstring_deserialize(&node->name, allocator, stream);
    binary_stream_read(
      stream, (uint8_t *)&node->transform,
      sizeof(matrix4f), sizeof(matrix4f));
    binary_stream_read(
      stream, (uint8_t *)&node->bone_index,
      sizeof(uint32_t), sizeof(uint32_t));
    cvector_def(&node->skel_nodes);
    cvector_deserialize(&node->skel_nodes, allocator, stream);
  }
}

void
bone_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    bone_t *bone = (bone_t *)dst;
    cstring_def(&bone->name);
    cstring_deserialize(&bone->name, allocator, stream);
    binary_stream_read(
      stream, (uint8_t *)&bone->offset_matrix,
      sizeof(matrix4f), sizeof(matrix4f));
    cvector_def(&bone->vertex_weights);
    cvector_deserialize(&bone->vertex_weights, allocator, stream);
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
    cvector_def(&skinned_mesh->bones);
    cvector_deserialize(&skinned_mesh->bones, allocator, stream);
    cvector_def(&skinned_mesh->skeleton.nodes);
    cvector_deserialize(&skinned_mesh->skeleton.nodes, allocator, stream);
  }
}

size_t
skel_node_type_size(void)
{
  return sizeof(skel_node_t);
}

size_t
bone_type_size(void)
{
  return sizeof(bone_t);
}

size_t
skinned_mesh_type_size(void)
{
  return sizeof(skinned_mesh_t);
}

uint32_t
skel_node_owns_alloc(void)
{
  return 0;
}

uint32_t
bone_owns_alloc(void)
{
  return 0;
}

uint32_t
skinned_mesh_owns_alloc(void)
{
  return 0;
}

const allocator_t *
skel_node_get_alloc(const void *ptr)
{
  return NULL;
}

const allocator_t *
bone_get_alloc(const void *ptr)
{
  return NULL;
}

const allocator_t *
skinned_mesh_get_alloc(const void *ptr)
{
  return NULL;
}

void
skel_node_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && allocator && !skel_node_is_def(ptr));

  {
    skel_node_t *node = (skel_node_t *)ptr;
    cstring_cleanup2(&node->name);
    cvector_cleanup2(&node->skel_nodes);
    memset(node, 0, sizeof(skel_node_t));
  }
}

void
bone_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && allocator && !bone_is_def(ptr));

  {
    bone_t *bone = (bone_t *)ptr;
    cstring_cleanup2(&bone->name);
    cvector_cleanup2(&bone->vertex_weights);
    memset(bone, 0, sizeof(bone_t));
  }
}

void
skinned_mesh_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && allocator && !skinned_mesh_is_def(ptr));

  {
    skinned_mesh_t *skinned_mesh = (skinned_mesh_t *)ptr;
    mesh_cleanup(&skinned_mesh->mesh, allocator);
    cvector_cleanup2(&skinned_mesh->bones);
    cvector_cleanup2(&skinned_mesh->skeleton.nodes);
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

INITIALIZER(register_skel_node_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = skel_node_def;
  vtable.fn_is_def = skel_node_is_def;
  vtable.fn_serialize = skel_node_serialize;
  vtable.fn_deserialize = skel_node_deserialize;
  vtable.fn_type_size = skel_node_type_size;
  vtable.fn_owns_alloc = skel_node_owns_alloc;
  vtable.fn_get_alloc = skel_node_get_alloc;
  vtable.fn_cleanup = skel_node_cleanup;
  register_type(get_type_id(skel_node_t), &vtable);
}

INITIALIZER(register_bone_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = bone_def;
  vtable.fn_is_def = bone_is_def;
  vtable.fn_serialize = bone_serialize;
  vtable.fn_deserialize = bone_deserialize;
  vtable.fn_type_size = bone_type_size;
  vtable.fn_owns_alloc = bone_owns_alloc;
  vtable.fn_get_alloc = bone_get_alloc;
  vtable.fn_cleanup = bone_cleanup;
  register_type(get_type_id(bone_t), &vtable);
}