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

// typedef
// struct scene_t {
//   cstring_t *name;
//   scene_metadata_t metadata;
//   node_repo_t node_repo;        // root is: node_repo.nodes[0];
//   light_repo_t light_repo;
//   mesh_repo_t mesh_repo;
//   material_repo_t material_repo;
//   texture_repo_t texture_repo;
//   font_repo_t font_repo;
//   camera_repo_t camera_repo;
//   bvh_repo_t bvh_repo;
// } scene_t;

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
    scene->name = (cstring_t *)allocator->mem_alloc(sizeof(cstring_t));
    cstring_def(scene->name);
    cstring_deserialize(scene->name, allocator, stream);

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
    scene_t *scene = (scene_t *)ptr;
    cstring_free(scene->name, allocator);

    free_mesh_array(scene->mesh_repo.meshes, scene->mesh_repo.count, allocator);
    free_node_array(scene->node_repo.nodes, scene->node_repo.count, allocator);
    free_font_array(scene->font_repo.fonts, scene->font_repo.count, allocator);

    if (scene->light_repo.count)
      free_light_array(
        scene->light_repo.lights, scene->light_repo.count, allocator);

    if (scene->material_repo.count)
      free_material_array(
        scene->material_repo.materials, scene->material_repo.count, allocator);
    
    if (scene->texture_repo.count)
      free_texture_array(
        scene->texture_repo.textures, scene->texture_repo.count, allocator);
    
    if (scene->camera_repo.count)
      allocator->mem_free(scene->camera_repo.cameras);

    if (scene->bvh_repo.count)
      free_bvh_array(scene->bvh_repo.bvhs, scene->bvh_repo.count, allocator);
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
  assert(name);

  scene->name = cstring_create(name, allocator);
}

scene_t*
scene_create(
  const char *name, 
  const allocator_t *allocator)
{
  assert(name && allocator);

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