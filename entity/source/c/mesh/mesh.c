/**
 * @file mesh.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2025-02-25
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <assert.h>
#include <string.h>
#include <entity/c/mesh/mesh.h>
#include <library/allocator/allocator.h>
#include <library/core/core.h>
#include <library/streams/binary_stream.h>
#include <library/type_registry/type_registry.h>
#include <math/c/common.h>
#include <math/c/matrix4f.h>


void
mesh_def(void *ptr)
{
  assert(ptr);

  {
    mesh_t *mesh = (mesh_t *)ptr;
    memset(mesh, 0, sizeof(mesh_t));
  }
}

uint32_t
mesh_is_def(const void *ptr)
{
  assert(ptr);

  {
    const mesh_t *mesh = (const mesh_t *)ptr;
    mesh_t def;
    mesh_def(&def);
    return !memcmp(mesh, &def, sizeof(mesh_t));
  }
}

void
mesh_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const mesh_t *mesh = (const mesh_t *)src;
    cvector_serialize(&mesh->vertices, stream);
    cvector_serialize(&mesh->normals, stream);
    cvector_serialize(&mesh->uvs, stream);
    cvector_serialize(&mesh->indices, stream);
    binary_stream_write(
      stream, mesh->materials.indices, sizeof(uint32_t) * MAX_MATERIAL_NUMBER);
  }
}

void
mesh_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    mesh_t *mesh = (mesh_t *)dst;
    cvector_def(&mesh->vertices);
    cvector_deserialize(&mesh->vertices, allocator, stream);
    cvector_def(&mesh->normals);
    cvector_deserialize(&mesh->normals, allocator, stream);
    cvector_def(&mesh->uvs);
    cvector_deserialize(&mesh->uvs, allocator, stream);
    cvector_def(&mesh->indices);
    cvector_deserialize(&mesh->indices, allocator, stream);
    binary_stream_read(
      stream, (uint8_t *)mesh->materials.indices,
      sizeof(uint32_t) * MAX_MATERIAL_NUMBER,
      sizeof(uint32_t) * MAX_MATERIAL_NUMBER);
  }
}

size_t
mesh_type_size(void)
{
  return sizeof(mesh_t);
}

uint32_t
mesh_owns_alloc(void)
{
  return 0;
}

const allocator_t *
mesh_get_alloc(const void *ptr)
{
  return NULL;
}

void
mesh_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !mesh_is_def(ptr));
  assert(allocator);

  {
    mesh_t *mesh = (mesh_t *)ptr;
    cvector_cleanup2(&mesh->vertices);
    cvector_cleanup2(&mesh->normals);
    cvector_cleanup2(&mesh->uvs);
    cvector_cleanup2(&mesh->indices);
    memset(mesh, 0, sizeof(mesh_t));
  }
}

////////////////////////////////////////////////////////////////////////////////
void
mesh_setup(
  mesh_t *mesh,
  float *vertices,
  float *normals,
  float *uvs,
  uint32_t vertices_count,
  uint32_t *indices,
  uint32_t indices_count,
  material_array_t materials,
  const allocator_t* allocator)
{
  assert(allocator);
  assert(mesh && mesh_is_def(mesh));
  assert(vertices && normals && uvs && indices);

  {
    size_t vtotal = sizeof(float) * vertices_count * 3;
    size_t itotal = sizeof(uint32_t) * indices_count;

    cvector_setup(&mesh->vertices, get_type_data(float), 0, allocator);
    cvector_resize(&mesh->vertices, vertices_count * 3);
    memcpy(mesh->vertices.data, vertices, vtotal);

    cvector_setup(&mesh->normals, get_type_data(float), 0, allocator);
    cvector_resize(&mesh->normals, vertices_count * 3);
    memcpy(mesh->normals.data, normals, vtotal);

    cvector_setup(&mesh->uvs, get_type_data(float), 0, allocator);
    cvector_resize(&mesh->uvs, vertices_count * 3);
    memcpy(mesh->uvs.data, uvs, vtotal);

    cvector_setup(&mesh->indices, get_type_data(uint32_t), 0, allocator);
    cvector_resize(&mesh->indices, indices_count);
    memcpy(mesh->indices.data, indices, itotal);

    mesh->materials = materials;
  }
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_mesh_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = mesh_def;
  vtable.fn_is_def = mesh_is_def;
  vtable.fn_serialize = mesh_serialize;
  vtable.fn_deserialize = mesh_deserialize;
  vtable.fn_type_size = mesh_type_size;
  vtable.fn_owns_alloc = mesh_owns_alloc;
  vtable.fn_get_alloc = mesh_get_alloc;
  vtable.fn_cleanup = mesh_cleanup;
  register_type(get_type_id(mesh_t), &vtable);
}