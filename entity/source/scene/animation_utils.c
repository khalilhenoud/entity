/**
 * @file animation_utils.c
 * @author khalilhenoud@gmail.com
 * @brief tmp: functionality for playing the animation (proof of concept)
 * @version 0.1
 * @date 2026-01-15
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <entity/mesh/skinned_mesh.h>
#include <entity/scene/animation.h>
#include <entity/scene/animation_utils.h>
#include <library/allocator/allocator.h>


typedef
struct anim_sequence_t {
  animation_t *anim;
  skinned_mesh_t *skinned_mesh;
  cvector_t nodes;                      // skel_node_t
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

    cvector_setup(&anim_sq->nodes, get_type_data(matrix4f), 0, _alloc);
    cvector_resize(&anim_sq->nodes, skinned_mesh->skeleton.nodes.size);

    return anim_sq;
  }
}

void
stop_anim(anim_sequence_t *anim_sq)
{
  {
    const allocator_t *_alloc = anim_sq->allocator;
    cvector_cleanup2(&anim_sq->nodes);
    _alloc->mem_free(anim_sq);
  }
}


static
uint32_t
find_translation(anim_node_t *channel, float time)
{
  assert(channel->position_keys.size);

  {
    cvector_t *keys = &channel->position_keys;
    for (uint32_t i = 0; i < keys->size - 1; ++i) {
      position_key_t *next_key = cvector_as(keys, i + 1, position_key_t);
      if (time < next_key->time)
        return i;
    }
  }
  return UINT32_MAX;
}

static
uint32_t
find_rotation(anim_node_t *channel, float time)
{
  assert(channel->rotation_keys.size);

  {
    cvector_t *keys = &channel->rotation_keys;
    for (uint32_t i = 0; i < keys->size - 1; ++i) {
      rotation_key_t *next_key = cvector_as(keys, i + 1, rotation_key_t);
      if (time < next_key->time)
        return i;
    }
  }
  return UINT32_MAX;
}

static
uint32_t
find_scale(anim_node_t *channel, float time)
{
  assert(channel->scale_keys.size);

  {
    cvector_t *keys = &channel->scale_keys;
    for (uint32_t i = 0; i < keys->size - 1; ++i) {
      scale_key_t *next_key = cvector_as(keys, i + 1, scale_key_t);
      if (time < next_key->time)
        return i;
    }
  }
  return UINT32_MAX;
}

static
anim_node_t *
find_anim_channel(animation_t *anim, cstring_t *bone_name)
{
  anim_node_t *node = NULL;
  for (uint32_t i = 0; i < anim->channels.size; ++i) {
    node = cvector_as(&anim->channels, i, anim_node_t);
    if (cstring_is_equal(&node->name, bone_name))
      return node;
  }

  return NULL;
}

static
void
update_bone_transforms(
  anim_sequence_t *anim_sq,
  float anim_time,
  skel_node_t *node,
  const uint32_t index);

void
update_anim(anim_sequence_t *anim_sq, float delta_t)
{
  animation_t *anim = anim_sq->anim;
  skinned_mesh_t *skinned_mesh = anim_sq->skinned_mesh;
  const float anim_tick_sec = anim->ticks_per_second;
  const float ticks_per_sec = anim_tick_sec != 0.f ? anim_tick_sec : 25.f;
  float time_in_ticks = (anim_sq->time += delta_t) * ticks_per_sec;
  float anim_time = fmod(time_in_ticks, anim->duration);

  skel_node_t *root = cvector_as(&skinned_mesh->skeleton.nodes, 0, skel_node_t);
  update_bone_transforms(anim_sq, anim_time, root, 0);
}

static
void
update_bone_transforms(
  anim_sequence_t *anim_sq,
  float anim_time,
  skel_node_t *node,
  const uint32_t index)
{
  animation_t *anim = anim_sq->anim;
  skinned_mesh_t *skinned_mesh = anim_sq->skinned_mesh;
  skeleton_t *skeleton = &skinned_mesh->skeleton;

  matrix4f node_transform = node->transform;
  matrix4f *inter_node = cvector_as(&anim_sq->nodes, index, matrix4f);

  // override node_transform if a valid channel exists
  anim_node_t *channel = find_anim_channel(anim_sq->anim, &node->name);
  if (channel && 1) {
    uint32_t indices;
    matrix4f s_r_t[3];
    matrix4f_set_identity(s_r_t + 0);
    matrix4f_set_identity(s_r_t + 1);
    matrix4f_set_identity(s_r_t + 2);

    indices = find_scale(channel, anim_time);
    if (indices != UINT32_MAX) {
      scale_key_t *k = cvector_as(&channel->scale_keys, indices, scale_key_t);
      vector3f scale = k->value;
      matrix4f_scale(s_r_t + 0, scale.data[0], scale.data[1], scale.data[2]);
    } else
      matrix4f_set_identity(s_r_t + 0);

    indices = find_rotation(channel, anim_time);
    if (indices != UINT32_MAX) {
      rotation_key_t *r = cvector_as(
        &channel->rotation_keys, indices, rotation_key_t);
      quatf rotation = r->value;
      vector3f axis;
      float angle;
      get_quatf_axis_angle(&rotation, &axis, &angle);
      angle = TO_DEGREES(angle);
      matrix4f_set_axisangle(s_r_t + 1, &axis, angle);
    } else
      matrix4f_set_identity(s_r_t + 1);

    indices = find_translation(channel, anim_time);
    if (indices != UINT32_MAX) {
      position_key_t *p = cvector_as(
        &channel->position_keys, indices, position_key_t);
      vector3f pos = p->value;
      matrix4f_translation(s_r_t + 2, pos.data[0], pos.data[1], pos.data[2]);
    } else
      matrix4f_set_identity(s_r_t + 2);

    {
      matrix4f intermediate;
      intermediate = mult_m4f(s_r_t + 1, s_r_t + 0);
      #if 1
      node_transform = mult_m4f(s_r_t + 2, &intermediate);
      #else
      // intermediate = mult_m4f(s_r_t + 2, &intermediate);
      // node_transform = mult_m4f(&intermediate, &node_transform);
      #endif
    }
  }

  *inter_node = node_transform;

  for (uint32_t i = 0; i < node->skel_nodes.size; ++i) {
    uint32_t child_index = *cvector_as(&node->skel_nodes, i, uint32_t);
    skel_node_t *child = cvector_as(&skeleton->nodes, child_index, skel_node_t);
    update_bone_transforms(
      anim_sq, anim_time, child, child_index);
  }
}

matrix4f
get_anim_bone_transform(anim_sequence_t *anim_sq, uint32_t index)
{
  return *cvector_as(&anim_sq->nodes, index, matrix4f);
}