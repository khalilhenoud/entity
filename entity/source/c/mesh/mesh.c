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
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <math/c/common.h>
#include <math/c/matrix4f.h>
#include <entity/c/mesh/mesh.h>


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
    size_t vtotal = sizeof(float) * 3 * mesh->vertices_count;
    size_t itotal = sizeof(uint32_t) * mesh->indices_count;
    size_t mtotal = sizeof(uint32_t) * MAX_MATERIAL_NUMBER;
    binary_stream_write(stream, &mesh->vertices_count, sizeof(uint32_t));
    binary_stream_write(stream, &mesh->indices_count, sizeof(uint32_t));
    binary_stream_write(stream, &mesh->materials.used, sizeof(uint32_t));
    binary_stream_write(stream, &mesh->materials.indices, mtotal);
    binary_stream_write(stream, &mesh->vertices, vtotal);
    binary_stream_write(stream, &mesh->normals, vtotal);
    binary_stream_write(stream, &mesh->uvs, vtotal);
    binary_stream_write(stream, &mesh->indices, itotal);
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
    size_t mtotal = sizeof(uint32_t) * MAX_MATERIAL_NUMBER;
    mesh_t *mesh = (mesh_t *)dst;
    binary_stream_read(
      stream, (uint8_t *)&mesh->vertices_count, 
      sizeof(uint32_t), sizeof(uint32_t));
    binary_stream_read(
      stream, (uint8_t *)&mesh->indices_count, 
      sizeof(uint32_t), sizeof(uint32_t));
    binary_stream_read(
      stream, (uint8_t *)&mesh->materials.used, 
      sizeof(uint32_t), sizeof(uint32_t));
    binary_stream_read(
      stream, (uint8_t *)&mesh->materials.indices, 
      mtotal, mtotal);
    {
      size_t vtotal = sizeof(float) * 3 * mesh->vertices_count;
      size_t itotal = sizeof(uint32_t) * mesh->indices_count;
      mesh->vertices = (float *)allocator->mem_alloc(vtotal);
      mesh->normals = (float *)allocator->mem_alloc(vtotal);
      mesh->uvs = (float *)allocator->mem_alloc(vtotal);
      mesh->indices = (uint32_t *)allocator->mem_alloc(itotal);
      binary_stream_read(stream, (uint8_t *)&mesh->vertices, vtotal, vtotal);
      binary_stream_read(stream, (uint8_t *)&mesh->normals, vtotal, vtotal);
      binary_stream_read(stream, (uint8_t *)&mesh->uvs, vtotal, vtotal);
      binary_stream_read(stream, (uint8_t *)&mesh->indices, itotal, itotal);
    }
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
    allocator->mem_free(mesh->vertices);
    allocator->mem_free(mesh->normals);
    allocator->mem_free(mesh->uvs);
    allocator->mem_free(mesh->indices);
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
    mesh->vertices_count = vertices_count;
    mesh->indices_count = indices_count;
    mesh->materials = materials;
    mesh->vertices = (float*)allocator->mem_alloc(vtotal);
    memcpy(mesh->vertices, vertices, vtotal);
    mesh->normals = (float*)allocator->mem_alloc(vtotal);
    memcpy(mesh->normals, normals, vtotal);
    mesh->uvs = (float*)allocator->mem_alloc(vtotal);
    memcpy(mesh->uvs, uvs, vtotal);
    mesh->indices = (uint32_t*)allocator->mem_alloc(itotal);
    memcpy(mesh->indices, indices, itotal);
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