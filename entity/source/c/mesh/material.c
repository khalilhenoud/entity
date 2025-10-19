/**
 * @file material.c
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
#include <entity/c/mesh/material.h>


void
material_def(void *ptr)
{
  assert(ptr);

  {
    material_t *material = (material_t *)ptr;
    memset(material, 0, sizeof(material_t));
  }
}

uint32_t 
material_is_def(const void *ptr)
{
  assert(ptr);

  {
    const material_t *material = (const material_t *)ptr;
    material_t def; 
    material_def(&def);
    return !memcmp(material, &def, sizeof(material_t));
  }
}

void 
material_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const material_t *material = (const material_t *)src;
    cstring_serialize(&material->name, stream);
    binary_stream_write(stream, &material->ambient, sizeof(color_rgba_t));
    binary_stream_write(stream, &material->diffuse, sizeof(color_rgba_t));
    binary_stream_write(stream, &material->specular, sizeof(color_rgba_t));
    binary_stream_write(stream, &material->shininess, sizeof(float));
    binary_stream_write(stream, &material->opacity, sizeof(float));
    binary_stream_write(stream, &material->textures.used, sizeof(uint32_t));
    {
      uint32_t i = 0;
      for (; i < material->textures.used; ++i) {
        const texture_properties_t *prop = material->textures.data + i;
        binary_stream_write(stream, &prop->index, sizeof(uint32_t));
        binary_stream_write(stream, &prop->u, sizeof(float));
        binary_stream_write(stream, &prop->v, sizeof(float));
        binary_stream_write(stream, &prop->u_scale, sizeof(float));
        binary_stream_write(stream, &prop->v_scale, sizeof(float));
        binary_stream_write(stream, &prop->angle, sizeof(float));
      }
    }
  }
}

void 
material_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    material_t *material = (material_t *)dst;
    cstring_def(&material->name);
    cstring_deserialize(&material->name, allocator, stream);
    binary_stream_read(
      stream, (uint8_t *)&material->ambient, 
      sizeof(color_rgba_t), sizeof(color_rgba_t));
    binary_stream_read(
      stream, (uint8_t *)&material->diffuse, 
      sizeof(color_rgba_t), sizeof(color_rgba_t));
    binary_stream_read(
      stream, (uint8_t *)&material->specular, 
      sizeof(color_rgba_t), sizeof(color_rgba_t));
    binary_stream_read(
      stream, (uint8_t *)&material->shininess, 
      sizeof(float), sizeof(float));
    binary_stream_read(
      stream, (uint8_t *)&material->opacity, 
      sizeof(float), sizeof(float));
    binary_stream_read(
      stream, (uint8_t *)&material->textures.used, 
      sizeof(uint32_t), sizeof(uint32_t));
    {
      uint32_t i = 0;
      for (; i < material->textures.used; ++i) {
        texture_properties_t *prop = material->textures.data + i;
        binary_stream_read(
          stream, (uint8_t *)&prop->index, 
          sizeof(uint32_t), sizeof(uint32_t));
        binary_stream_read(
          stream, (uint8_t *)&prop->u, 
          sizeof(float), sizeof(float));
        binary_stream_read(
          stream, (uint8_t *)&prop->v, 
          sizeof(float), sizeof(float));
        binary_stream_read(
          stream, (uint8_t *)&prop->u_scale, 
          sizeof(float), sizeof(float));
        binary_stream_read(
          stream, (uint8_t *)&prop->v_scale, 
          sizeof(float), sizeof(float));
        binary_stream_read(
          stream, (uint8_t *)&prop->angle, 
          sizeof(float), sizeof(float));
      }
    }
  }
}

size_t 
material_type_size(void)
{
  return sizeof(material_t);
}

uint32_t 
material_owns_alloc(void)
{
  return 0;
}

const allocator_t *
material_get_alloc(const void *ptr)
{
  return NULL;
}

void 
material_cleanup(
  void *ptr, 
  const allocator_t *allocator)
{
  assert(ptr && !material_is_def(ptr));
  assert(allocator);

  {
    material_t *material = (material_t *)ptr;
    cstring_cleanup2(&material->name);
  }
}

////////////////////////////////////////////////////////////////////////////////
void
material_setup(
  material_t *material, 
  const char *name, 
  color_rgba_t ambient,
  color_rgba_t diffuse,
  color_rgba_t specular,
  float shininess,
  float opacity,
  texture_properties_array_t textures,
  const allocator_t* allocator)
{
  assert(allocator);
  assert(material && material_is_def(material));
  assert(name);

  cstring_def(&material->name);
  cstring_setup(&material->name, name, allocator);
  material->ambient = ambient;
  material->diffuse = diffuse;
  material->specular = specular;
  material->shininess = shininess;
  material->opacity = opacity;
  material->textures = textures;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_material_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = material_def;
  vtable.fn_is_def = material_is_def;
  vtable.fn_serialize = material_serialize;
  vtable.fn_deserialize = material_deserialize;
  vtable.fn_type_size = material_type_size;
  vtable.fn_owns_alloc = material_owns_alloc;
  vtable.fn_get_alloc = material_get_alloc;
  vtable.fn_cleanup = material_cleanup;
  register_type(get_type_id(material_t), &vtable);
}