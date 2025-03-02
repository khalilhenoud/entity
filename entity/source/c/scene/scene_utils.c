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
#include <library/string/cstring.h>
#include <entity/c/scene/scene.h>
#include <entity/c/scene/scene_utils.h>


scene_t*
create_scene(
  const char* name, 
  const allocator_t* allocator)
{
  return scene_create(name, allocator);
}

void
free_scene(scene_t* scene, const allocator_t* allocator)
{
  scene_free(scene, allocator);
}