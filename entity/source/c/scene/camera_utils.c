/**
 * @file camera_utils.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include <library/allocator/allocator.h>
#include <math/c/matrix4f.h>
#include <entity/c/scene/camera.h>
#include <entity/c/scene/camera_utils.h>


camera_t*
create_camera(const allocator_t* allocator)
{
  assert(allocator);
  
  {
    camera_t* camera = (camera_t*)allocator->mem_alloc(sizeof(camera_t));
    memset(camera, 0, sizeof(camera_t));
    return camera;
  }
}

void
free_camera(camera_t* camera, const allocator_t* allocator)
{
  assert(camera);
  assert(allocator);
  allocator->mem_free(camera);
}

////////////////////////////////////////////////////////////////////////////////
void
set_camera_lookat(
  camera_t* camera, 
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
get_view_transformation(
  const camera_t* camera, 
  matrix4f* out)
{
  assert(camera);
  assert(out && "out matrix cannot be null!");

  matrix3f_set_identity(out);

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