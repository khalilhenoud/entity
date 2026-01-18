/**
 * @file animation_utils.c
 * @author khalilhenoud@gmail.com
 * @brief provide functionality for playing the animation (proof of concept)
 * @version 0.1
 * @date 2026-01-15
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <entity/mesh/mesh.h>
#include <entity/mesh/skinned_mesh.h>
#include <entity/scene/animation.h>
#include <entity/scene/animation_utils.h>
#include <library/allocator/allocator.h>


// NOTE: this is intended only as a proof of concept and not as a final library
typedef
struct anim_sequence_t {
  animation_t *anim;
  skinned_mesh_t *skinned_mesh;
  mesh_t *mesh;
  const allocator_t *allocator;
  float time;
} anim_sequence_t;

anim_sequence_t *
play_anim(
  animation_t *anim,
  skinned_mesh_t *skinned_mesh,
  const allocator_t *allocator)
{
  assert(anim && skinned_mesh);

  {
    const allocator_t *_alloc = allocator ? allocator : &g_default_allocator;
    anim_sequence_t *anim_sq = _alloc->mem_alloc(sizeof(anim_sequence_t));
    mesh_t *mesh = &skinned_mesh->mesh;
    anim_sq->allocator = _alloc;
    anim_sq->anim = anim;
    anim_sq->skinned_mesh = skinned_mesh;
    anim_sq->time = 0.f;

    // duplicate mesh_t found in skinned_mesh, this will be transformed every
    // frame, is highly inefficient, but for now is used as a proof of concept
    // skinned_mesh->mesh
    anim_sq->mesh = _alloc->mem_alloc(sizeof(mesh_t));
    mesh_def(anim_sq->mesh);
    mesh_setup(
      anim_sq->mesh,
      (float *)mesh->vertices.data,
      (float *)mesh->normals.data,
      (float *)mesh->uvs.data,
      (uint32_t)mesh->vertices.size / 3,
      (uint32_t *)mesh->indices.data,
      mesh->indices.size,
      mesh->materials,
      _alloc);

    return anim_sq;
  }
}

void
stop_anim(anim_sequence_t *anim_sq)
{
  assert(anim_sq);

  {
    const allocator_t *_alloc = anim_sq->allocator;
    mesh_cleanup(anim_sq->mesh, _alloc);
    _alloc->mem_free(anim_sq->mesh);
    _alloc->mem_free(anim_sq);
  }
}

void
update_anim(anim_sequence_t *anim_sq, float delta_t)
{
  assert(anim_sq);
}

mesh_t *
get_anim_mesh(anim_sequence_t *anim_sq)
{
  assert(anim_sq && anim_sq->mesh);
  return anim_sq->mesh;
}