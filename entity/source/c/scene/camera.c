/**
 * @file camera.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <assert.h>
#include <string.h>
#include <math/c/matrix4f.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <entity/c/scene/camera.h>


void
camera_def(void *ptr)
{
  assert(ptr);

  {
    camera_t *camera = (camera_t *)ptr;
    memset(camera, 0, sizeof(camera_t));
  }
}

uint32_t 
camera_is_def(const void *ptr)
{
  assert(ptr);

  {
    const camera_t *camera = (const camera_t *)ptr;
    camera_t def; 
    camera_def(&def);
    return !memcmp(camera, &def, sizeof(camera_t));
  }
}

void 
camera_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const camera_t *camera = (const camera_t *)src;
    binary_stream_write(stream, &camera->position, sizeof(vector3f));
    binary_stream_write(stream, &camera->lookat_direction, sizeof(vector3f));
    binary_stream_write(stream, &camera->up_vector, sizeof(vector3f));
  }
}

void 
camera_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    size_t sv3 = sizeof(vector3f);
    camera_t *camera = (camera_t *)dst;
    binary_stream_read(stream, (uint8_t *)&camera->position, sv3, sv3);
    binary_stream_read(stream, (uint8_t *)&camera->lookat_direction, sv3, sv3);
    binary_stream_read(stream, (uint8_t *)&camera->up_vector, sv3, sv3);
  }
}

size_t 
camera_type_size(void)
{
  return sizeof(camera_t);
}

////////////////////////////////////////////////////////////////////////////////
void
camera_setup(
  camera_t *camera, 
  const vector3f position, 
  const vector3f lookat, 
  const vector3f upvector)
{
  assert(camera && camera_is_def(camera));
  camera_set_lookat(camera, position, lookat, upvector);
}

camera_t*
camera_create(const allocator_t* allocator)
{
  assert(allocator);
  
  {
    camera_t* camera = (camera_t*)allocator->mem_alloc(sizeof(camera_t));
    camera_def(camera);
    return camera;
  }
}

void
camera_free(
  camera_t* camera, 
  const allocator_t* allocator)
{
  assert(camera && allocator);
  allocator->mem_free(camera);
}

void
camera_set_lookat(
  camera_t *camera, 
  const vector3f position, 
  const vector3f lookat, 
  const vector3f upvector)
{
  assert(camera);

  camera->lookat_direction = diff_v3f(&position, &lookat);
  normalize_set_v3f(&camera->lookat_direction);
  camera->up_vector = upvector;
  normalize_set_v3f(&camera->up_vector);
  camera->position = position;
}

void
camera_view_matrix(
  const camera_t *camera, 
  matrix4f *out)
{
  assert(camera && out);

  matrix4f_set_identity(out);

  {
    matrix4f translation, cross_product, camera_rotation;
    vector3f tmp, result;
    matrix4f_set_identity(&camera_rotation);
    matrix4f_translation(
      &translation, 
      -camera->position.data[0], 
      -camera->position.data[1], 
      -camera->position.data[2]);
    matrix4f_cross_product(&cross_product, &camera->up_vector);
    tmp = camera->lookat_direction;
    mult_set_v3f(&tmp, -1.f);
    result = mult_m4f_v3f(&cross_product, &tmp);
    camera_rotation.data[M4_RC_00] = result.data[0];
    camera_rotation.data[M4_RC_01] = result.data[1];
    camera_rotation.data[M4_RC_02] = result.data[2];
    camera_rotation.data[M4_RC_10] = camera->up_vector.data[0];
    camera_rotation.data[M4_RC_11] = camera->up_vector.data[1];
    camera_rotation.data[M4_RC_12] = camera->up_vector.data[2];
    camera_rotation.data[M4_RC_20] = tmp.data[0];
    camera_rotation.data[M4_RC_21] = tmp.data[1];
    camera_rotation.data[M4_RC_22] = tmp.data[2];
    matrix4f_copy(out, &camera_rotation);
    mult_set_m4f(out, &translation);
  }
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_camera_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = camera_def;
  vtable.fn_is_def = camera_is_def;
  vtable.fn_serialize = camera_serialize;
  vtable.fn_deserialize = camera_deserialize;
  vtable.fn_type_size = camera_type_size;
  register_type(get_type_id(camera_t), &vtable);
}