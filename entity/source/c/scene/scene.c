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
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <library/string/cstring.h>
#include <library/streams/binary_stream.h>
#include <entity/c/scene/scene.h>
#include <entity/c/scene/camera.h>
#include <entity/c/scene/camera_utils.h>
#include <entity/c/scene/light.h>
#include <entity/c/scene/light_utils.h>
#include <entity/c/scene/node.h>
#include <entity/c/scene/node_utils.h>
#include <entity/c/mesh/mesh.h>
#include <entity/c/mesh/mesh_utils.h>
#include <entity/c/mesh/material.h>
#include <entity/c/mesh/material_utils.h>
#include <entity/c/mesh/texture.h>
#include <entity/c/mesh/texture_utils.h>
#include <entity/c/misc/font.h>
#include <entity/c/misc/font_utils.h>
#include <entity/c/spatial/bvh.h>
#include <entity/c/spatial/bvh_utils.h>


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
    cstring_serialize(scene->name, stream);
    binary_stream_write(stream, &scene->metadata, sizeof(scene_metadata_t));

    binary_stream_write(stream, &scene->node_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->node_repo.count; ++i)
      node_serialize(scene->node_repo.nodes + i, stream);

    binary_stream_write(stream, &scene->light_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->light_repo.count; ++i)
      light_serialize(scene->light_repo.lights + i, stream);

    binary_stream_write(stream, &scene->mesh_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->mesh_repo.count; ++i)
      mesh_serialize(scene->mesh_repo.meshes + i, stream);

    binary_stream_write(stream, &scene->material_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->material_repo.count; ++i)
      material_serialize(scene->material_repo.materials + i, stream);

    binary_stream_write(stream, &scene->texture_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->texture_repo.count; ++i)
      texture_serialize(scene->texture_repo.textures + i, stream);

    binary_stream_write(stream, &scene->font_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->font_repo.count; ++i)
      font_serialize(scene->font_repo.fonts + i, stream);

    binary_stream_write(stream, &scene->camera_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->camera_repo.count; ++i)
      camera_serialize(scene->camera_repo.cameras + i, stream);

    binary_stream_write(stream, &scene->bvh_repo.count, sizeof(uint32_t));
    for (i = 0; i < scene->bvh_repo.count; ++i)
      bvh_serialize(scene->bvh_repo.bvhs + i, stream);
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
    cstring_cleanup(scene->name, NULL);
    cstring_deserialize(scene->name, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->metadata, 
      sizeof(scene_metadata_t), sizeof(scene_metadata_t));

    binary_stream_read(
      stream, (uint8_t *)&scene->node_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->node_repo.nodes = (node_t *)allocator->mem_alloc(
      scene->node_repo.count * sizeof(node_t));
    for (i = 0; i < scene->node_repo.count; ++i)
      node_deserialize(scene->node_repo.nodes + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->light_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->light_repo.lights = (light_t *)allocator->mem_alloc(
      scene->light_repo.count * sizeof(light_t));
    for (i = 0; i < scene->light_repo.count; ++i)
      light_deserialize(scene->light_repo.lights + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->mesh_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->mesh_repo.meshes = (mesh_t *)allocator->mem_alloc(
      scene->mesh_repo.count * sizeof(mesh_t));
    for (i = 0; i < scene->mesh_repo.count; ++i)
      mesh_deserialize(scene->mesh_repo.meshes + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->material_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->material_repo.materials = (material_t *)allocator->mem_alloc(
      scene->material_repo.count * sizeof(material_t));
    for (i = 0; i < scene->material_repo.count; ++i)
      material_deserialize(
        scene->material_repo.materials + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->texture_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->texture_repo.textures = (texture_t *)allocator->mem_alloc(
      scene->texture_repo.count * sizeof(texture_t));
    for (i = 0; i < scene->texture_repo.count; ++i)
      texture_deserialize(scene->texture_repo.textures + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->font_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->font_repo.fonts = (font_t *)allocator->mem_alloc(
      scene->font_repo.count * sizeof(font_t));
    for (i = 0; i < scene->font_repo.count; ++i)
      font_deserialize(scene->font_repo.fonts + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->camera_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->camera_repo.cameras = (camera_t *)allocator->mem_alloc(
      scene->camera_repo.count * sizeof(camera_t));
    for (i = 0; i < scene->camera_repo.count; ++i)
      camera_deserialize(scene->camera_repo.cameras + i, allocator, stream);

    binary_stream_read(
      stream, (uint8_t *)&scene->bvh_repo.count, 
      sizeof(uint32_t), sizeof(uint32_t));
    scene->bvh_repo.bvhs = (bvh_t *)allocator->mem_alloc(
      scene->bvh_repo.count * sizeof(bvh_t));
    for (i = 0; i < scene->bvh_repo.count; ++i)
      bvh_deserialize(scene->bvh_repo.bvhs + i, allocator, stream);
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
    cstring_free(scene->name, allocator);

    for (i = 0; i < scene->mesh_repo.count; ++i)
      mesh_cleanup(scene->mesh_repo.meshes + i, allocator);
    allocator->mem_free(scene->mesh_repo.meshes);

    for (i = 0; i < scene->node_repo.count; ++i)
      node_cleanup(scene->node_repo.nodes + i, allocator);
    allocator->mem_free(scene->node_repo.nodes);

    for (i = 0; i < scene->font_repo.count; ++i)
      font_cleanup(scene->font_repo.fonts + i, allocator);
    allocator->mem_free(scene->font_repo.fonts);

    for (i = 0; i < scene->light_repo.count; ++i)
      light_cleanup(scene->light_repo.lights + i, allocator);
    allocator->mem_free(scene->light_repo.lights);

    for (i = 0; i < scene->material_repo.count; ++i)
      material_cleanup(scene->material_repo.materials + i, allocator);
    allocator->mem_free(scene->material_repo.materials);

    for (i = 0; i < scene->texture_repo.count; ++i)
      texture_cleanup(scene->texture_repo.textures + i, allocator);
    allocator->mem_free(scene->texture_repo.textures);

    allocator->mem_free(scene->camera_repo.cameras);

    for (i = 0; i < scene->bvh_repo.count; ++i)
      bvh_cleanup(scene->bvh_repo.bvhs + i, allocator);
    allocator->mem_free(scene->bvh_repo.bvhs);
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

  scene->name = cstring_create(name, allocator);
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