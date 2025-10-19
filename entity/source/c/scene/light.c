/**
 * @file light.c
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
#include <entity/c/scene/light.h>


void
light_def(void *ptr)
{
  assert(ptr);

  {
    light_t *light = (light_t *)ptr;
    memset(light, 0, sizeof(light_t));
  }
}

uint32_t 
light_is_def(const void *ptr)
{
  assert(ptr);

  {
    const light_t *light = (const light_t *)ptr;
    light_t def; 
    light_def(&def);
    return !memcmp(light, &def, sizeof(light_t));
  }
}

void 
light_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const light_t *light = (const light_t *)src;
    cstring_serialize(&light->name, stream);
    binary_stream_write(stream, &light->position, sizeof(vector3f));
    binary_stream_write(stream, &light->direction, sizeof(vector3f));
    binary_stream_write(stream, &light->up, sizeof(vector3f));
    binary_stream_write(stream, &light->inner_cone, sizeof(float));
    binary_stream_write(stream, &light->outer_cone, sizeof(float));
    binary_stream_write(stream, &light->attenuation_constant, sizeof(float));
    binary_stream_write(stream, &light->attenuation_linear, sizeof(float));
    binary_stream_write(stream, &light->attenuation_quadratic, sizeof(float));
    binary_stream_write(stream, &light->diffuse, sizeof(color_rgba_t));
    binary_stream_write(stream, &light->specular, sizeof(color_rgba_t));
    binary_stream_write(stream, &light->ambient, sizeof(color_rgba_t));
    binary_stream_write(stream, &light->type, sizeof(light_type_t));
  }
}

void 
light_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    light_t *light = (light_t *)dst;
    cstring_def(&light->name);
    cstring_deserialize(&light->name, allocator, stream);
    binary_stream_read(stream, 
      (uint8_t *)&light->position, sizeof(vector3f), sizeof(vector3f));
    binary_stream_read(stream, 
      (uint8_t *)&light->direction, sizeof(vector3f), sizeof(vector3f));
    binary_stream_read(stream, 
      (uint8_t *)&light->up, sizeof(vector3f), sizeof(vector3f));
    binary_stream_read(stream, 
      (uint8_t *)&light->inner_cone, sizeof(float), sizeof(float));
    binary_stream_read(stream, 
      (uint8_t *)&light->outer_cone, sizeof(float), sizeof(float));
    binary_stream_read(stream, 
      (uint8_t *)&light->attenuation_constant, sizeof(float), sizeof(float));
    binary_stream_read(stream, 
      (uint8_t *)&light->attenuation_linear, sizeof(float), sizeof(float));
    binary_stream_read(stream, 
      (uint8_t *)&light->attenuation_quadratic, sizeof(float), sizeof(float));
    binary_stream_read(stream, 
      (uint8_t *)&light->diffuse, sizeof(color_rgba_t), sizeof(color_rgba_t));
    binary_stream_read(stream, 
      (uint8_t *)&light->specular, sizeof(color_rgba_t), sizeof(color_rgba_t));
    binary_stream_read(stream, 
      (uint8_t *)&light->ambient, sizeof(color_rgba_t), sizeof(color_rgba_t));
    binary_stream_read(stream, 
      (uint8_t *)&light->type, sizeof(light_type_t), sizeof(light_type_t));
  }
}

size_t 
light_type_size(void)
{
  return sizeof(light_t);
}

uint32_t 
light_owns_alloc(void)
{
  return 0;
}

const allocator_t *
light_get_alloc(const void *ptr)
{
  return NULL;
}

void 
light_cleanup(
  void *ptr, 
  const allocator_t *allocator)
{
  assert(ptr && !light_is_def(ptr));
  assert(allocator);

  {
    light_t *light = (light_t *)ptr;
    cstring_cleanup2(&light->name);
  }
}

////////////////////////////////////////////////////////////////////////////////
void
light_setup(
  light_t *light, 
  const char *name, 
  vector3f position,
  vector3f direction,
  vector3f up,
  float inner_cone,
  float outer_cone,
  float attenuation_constant,
  float attenuation_linear,
  float attenuation_quadratic,
  color_rgba_t diffuse,
  color_rgba_t specular,
  color_rgba_t ambient,
  light_type_t type,
  const allocator_t *allocator)
{
  assert(allocator);
  assert(light && light_is_def(light));
  assert(name);

  cstring_setup(&light->name, name, allocator);
  light->position = position;
  light->direction = direction;
  light->up = up;
  light->inner_cone = inner_cone;
  light->outer_cone = outer_cone;
  light->attenuation_constant = attenuation_constant;
  light->attenuation_linear = attenuation_linear;
  light->attenuation_quadratic = attenuation_quadratic;
  light->diffuse = diffuse;
  light->specular = specular;
  light->ambient = ambient;
  light->type = type;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_light_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = light_def;
  vtable.fn_is_def = light_is_def;
  vtable.fn_serialize = light_serialize;
  vtable.fn_deserialize = light_deserialize;
  vtable.fn_type_size = light_type_size;
  vtable.fn_owns_alloc = light_owns_alloc;
  vtable.fn_get_alloc = light_get_alloc;
  vtable.fn_cleanup = light_cleanup;
  register_type(get_type_id(light_t), &vtable);
}