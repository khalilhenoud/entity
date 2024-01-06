/**
 * @file scene_utils.c
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
#include <library/string/string.h>
#include <entity/c/scene/scene.h>
#include <entity/c/scene/scene_utils.h>
#include <entity/c/scene/node_utils.h>
#include <entity/c/mesh/mesh_utils.h>
#include <entity/c/mesh/material.h>
#include <entity/c/mesh/material_utils.h>
#include <entity/c/mesh/texture.h>
#include <entity/c/mesh/texture_utils.h>
#include <entity/c/misc/font.h>
#include <entity/c/misc/font_utils.h>


scene_t*
create_scene(
  const char* name, 
  const allocator_t* allocator)
{
  assert(name && allocator);

  {
    scene_t* scene = allocator->mem_alloc(sizeof(scene_t));
    assert(scene && "failed to allocate scene!");
    
    memset(scene, 0, sizeof(scene));
    scene->name = allocate_string(name, allocator);

    return scene;
  }
}

void
free_scene(scene_t* scene, const allocator_t* allocator)
{
  assert(scene && allocator);

  free_string(scene->name);
  free_mesh_array(scene->mesh_repo.meshes, scene->mesh_repo.count, allocator);
  free_node_array(scene->node_repo.nodes, scene->node_repo.count, allocator);
  free_font_array(scene->font_repo.fonts, scene->font_repo.count, allocator);

  if (scene->material_repo.count)
    free_material_array(
      scene->material_repo.materials, scene->material_repo.count, allocator);
  
  if (scene->texture_repo.count)
    free_texture_array(
      scene->texture_repo.textures, scene->texture_repo.count, allocator);
  
  if (scene->camera_repo.count)
    allocator->mem_free(scene->camera_repo.cameras);

  allocator->mem_free(scene);
}