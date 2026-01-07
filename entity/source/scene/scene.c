/**
 * @file scene.c
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
#include <entity/mesh/material.h>
#include <entity/mesh/mesh.h>
#include <entity/mesh/mesh_utils.h>
#include <entity/mesh/skinned_mesh.h>
#include <entity/mesh/texture.h>
#include <entity/misc/font.h>
#include <entity/scene/animation.h>
#include <entity/scene/camera.h>
#include <entity/scene/light.h>
#include <entity/scene/node.h>
#include <entity/scene/scene.h>
#include <entity/spatial/bvh.h>
#include <library/allocator/allocator.h>
#include <library/core/core.h>
#include <library/streams/binary_stream.h>
#include <library/type_registry/type_registry.h>


void
scene_def(void *ptr)
{
  assert(ptr);

  {
    scene_t *scene = (scene_t *)ptr;
    memset(scene, 0, sizeof(scene_t));
  }
}

uint32_t
scene_is_def(const void *ptr)
{
  assert(ptr);

  {
    const scene_t *scene = (const scene_t *)ptr;
    scene_t def;
    scene_def(&def);
    return !memcmp(scene, &def, sizeof(scene_t));
  }
}

void
scene_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    uint32_t i = 0;
    const scene_t *scene = (const scene_t *)src;
    cstring_serialize(&scene->name, stream);
    binary_stream_write(stream, &scene->metadata, sizeof(scene_metadata_t));

    cvector_serialize(&scene->node_repo, stream);
    cvector_serialize(&scene->animation_repo, stream);
    cvector_serialize(&scene->light_repo, stream);
    cvector_serialize(&scene->mesh_repo, stream);
    cvector_serialize(&scene->skinned_mesh_repo, stream);
    cvector_serialize(&scene->material_repo, stream);
    cvector_serialize(&scene->texture_repo, stream);
    cvector_serialize(&scene->font_repo, stream);
    cvector_serialize(&scene->camera_repo, stream);
    cvector_serialize(&scene->bvh_repo, stream);
  }
}

void
scene_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    uint32_t i = 0;
    scene_t *scene = (scene_t *)dst;
    cstring_cleanup2(&scene->name);
    cstring_deserialize(&scene->name, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->metadata,
      sizeof(scene_metadata_t), sizeof(scene_metadata_t));

    cvector_deserialize(&scene->node_repo, allocator, stream);
    cvector_deserialize(&scene->animation_repo, allocator, stream);
    cvector_deserialize(&scene->light_repo, allocator, stream);
    cvector_deserialize(&scene->mesh_repo, allocator, stream);
    cvector_deserialize(&scene->skinned_mesh_repo, allocator, stream);
    cvector_deserialize(&scene->material_repo, allocator, stream);
    cvector_deserialize(&scene->texture_repo, allocator, stream);
    cvector_deserialize(&scene->font_repo, allocator, stream);
    cvector_deserialize(&scene->camera_repo, allocator, stream);
    cvector_deserialize(&scene->bvh_repo, allocator, stream);
  }
}

size_t
scene_type_size(void)
{
  return sizeof(scene_t);
}

uint32_t
scene_owns_alloc(void)
{
  return 0;
}

const allocator_t *
scene_get_alloc(const void *ptr)
{
  return NULL;
}

void
scene_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !scene_is_def(ptr));
  assert(allocator);

  {
    uint32_t i = 0;
    scene_t *scene = (scene_t *)ptr;
    cstring_cleanup2(&scene->name);
    cvector_cleanup2(&scene->mesh_repo);
    cvector_cleanup2(&scene->skinned_mesh_repo);
    cvector_cleanup2(&scene->node_repo);
    cvector_cleanup2(&scene->animation_repo);
    cvector_cleanup2(&scene->font_repo);
    cvector_cleanup2(&scene->light_repo);
    cvector_cleanup2(&scene->material_repo);
    cvector_cleanup2(&scene->texture_repo);
    cvector_cleanup2(&scene->camera_repo);
    cvector_cleanup2(&scene->bvh_repo);
  }
}

////////////////////////////////////////////////////////////////////////////////
void
scene_setup(
  scene_t *scene,
  const char *name,
  const allocator_t* allocator)
{
  assert(allocator);
  assert(scene && scene_is_def(scene));

  cstring_setup2(&scene->name, name);
}

scene_t*
scene_create(
  const char *name,
  const allocator_t *allocator)
{
  assert(allocator);

  {
    scene_t *scene = (scene_t *)allocator->mem_alloc(sizeof(scene_t));
    scene_def(scene);
    scene_setup(scene, name, allocator);
    return scene;
  }
}

void
scene_free(
  scene_t *scene,
  const allocator_t* allocator)
{
  assert(scene && allocator);
  scene_cleanup(scene, allocator);
  allocator->mem_free(scene);
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_scene_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = scene_def;
  vtable.fn_is_def = scene_is_def;
  vtable.fn_serialize = scene_serialize;
  vtable.fn_deserialize = scene_deserialize;
  vtable.fn_type_size = scene_type_size;
  vtable.fn_owns_alloc = scene_owns_alloc;
  vtable.fn_get_alloc = scene_get_alloc;
  vtable.fn_cleanup = scene_cleanup;
  register_type(get_type_id(scene_t), &vtable);
}