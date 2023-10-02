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
#include <entity/c/scene/scene.h>
#include <entity/c/scene/scene_utils.h>
#include <entity/c/scene/node_utils.h>
#include <entity/c/mesh/mesh_utils.h>


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
    memcpy(scene->name.data, name, strlen(name));

    return scene;
  }
}

void
free_scene(scene_t* scene, const allocator_t* allocator)
{
  assert(scene && allocator);

  free_mesh_array(scene->mesh_repo.meshes, scene->mesh_repo.count, allocator);
  free_node_array(scene->node_repo.nodes, scene->node_repo.count, allocator);
  
  // TODO: Replace by their own functions.
  if (scene->material_repo.count)
    allocator->mem_free(scene->material_repo.materials);
  if (scene->texture_repo.count)
    allocator->mem_free(scene->texture_repo.textures);
  if (scene->font_repo.count)
    allocator->mem_free(scene->font_repo.fonts);
  if (scene->camera_repo.count)
    allocator->mem_free(scene->camera_repo.cameras);

  allocator->mem_free(scene);
}