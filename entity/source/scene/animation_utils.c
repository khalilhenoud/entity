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
  cvector_t local_transforms;        // node local transforms
  cvector_t final_transforms;        // bone final transforms
  cvector_t vertices;
  cvector_t normals;
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

    cvector_setup(&anim_sq->vertices, get_type_data(float), 0, _alloc);
    cvector_resize(&anim_sq->vertices, mesh->vertices.size);

    cvector_setup(&anim_sq->normals, get_type_data(float), 0, _alloc);
    cvector_resize(&anim_sq->normals, mesh->normals.size);

    cvector_setup(
      &anim_sq->local_transforms, get_type_data(matrix4f), 0, _alloc);
    cvector_resize(
      &anim_sq->local_transforms, skinned_mesh->skeleton.nodes.size);

    cvector_setup(
      &anim_sq->final_transforms, get_type_data(matrix4f), 0, _alloc);
    cvector_resize(
      &anim_sq->final_transforms, skinned_mesh->bones.size);

    return anim_sq;
  }
}

void
stop_anim(anim_sequence_t *anim_sq)
{
  {
    const allocator_t *_alloc = anim_sq->allocator;
    cvector_cleanup2(&anim_sq->vertices);
    cvector_cleanup2(&anim_sq->normals);
    cvector_cleanup2(&anim_sq->local_transforms);
    cvector_cleanup2(&anim_sq->final_transforms);
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
  const matrix4f *root_inverse,
  skel_node_t *node,
  const matrix4f *parent_transform,
  const uint32_t index,
  const uint32_t root_motion);

static
void
update_vertices(anim_sequence_t *anim_sq);

void
update_anim(
  anim_sequence_t *anim_sq,
  float delta_t,
  uint32_t root_motion)
{
  animation_t *anim = anim_sq->anim;
  skinned_mesh_t *skinned_mesh = anim_sq->skinned_mesh;
  const float anim_tick_sec = anim->ticks_per_second;
  const float ticks_per_sec = anim_tick_sec != 0.f ? anim_tick_sec : 25.f;
  float time_in_ticks = (anim_sq->time += delta_t) * ticks_per_sec;
  float anim_time = fmod(time_in_ticks, anim->duration);

  skel_node_t *root = cvector_as(&skinned_mesh->skeleton.nodes, 0, skel_node_t);
  matrix4f root_inverse = inverse_m4f(&root->transform);
  matrix4f identity;
  matrix4f_set_identity(&identity);
  update_bone_transforms(
    anim_sq,
    anim_time,
    &root_inverse,
    root,
    &identity,
    0,
    root_motion);

  update_vertices(anim_sq);
}

static
void
update_vertices(anim_sequence_t *anim_sq)
{
  animation_t *anim = anim_sq->anim;
  skinned_mesh_t *skinned_mesh = anim_sq->skinned_mesh;
  mesh_t *mesh = &skinned_mesh->mesh;
  cvector_t *vertex_to_bones = &skinned_mesh->vertex_to_bones;

  float *vertices = (float *)anim_sq->vertices.data;
  float *normals = (float *)anim_sq->normals.data;
  float *original = (float *)mesh->vertices.data;
  float *original_normals = (float *)mesh->normals.data;
  for (uint32_t i = 0, size = mesh->vertices.size / 3; i < size; ++i) {
    float total_weight = 0.f;
    uint32_t idx = i * 3;
    point3f vertex = {
      original[idx],
      original[idx + 1],
      original[idx + 2] };
    point3f normal = {
      original_normals[idx],
      original_normals[idx + 1],
      original_normals[idx + 2] };
    point3f skinned = { 0.f, 0.f, 0.f };
    point3f skinned_normal = { 0.f, 0.f, 0.f };

    cvector_t *to_bones = cvector_as(vertex_to_bones, i, cvector_t);
    vertices[i * 3 + 0] = vertices[i * 3 + 1] = vertices[i * 3 + 2] = 0.f;
    normals[i * 3 + 0] = normals[i * 3 + 1] = normals[i * 3 + 2] = 0.f;

    for (uint32_t j = 0; j < to_bones->size; ++j) {
      bone_weight_t *bone_weight = cvector_as(to_bones, j, bone_weight_t);
      matrix4f *final_transform = cvector_as(
        &anim_sq->final_transforms, bone_weight->id, matrix4f);
      point3f intermediate = { 0.f, 0.f, 0.f };
      matrix4f inverse_transpose = *final_transform;
      point3f intermediate_normal = { 0.f, 0.f, 0.f };

      inverse_set_m4f(&inverse_transpose);
      transpose_set_m4f(&inverse_transpose);

      intermediate = mult_m4f_p3f(final_transform, &vertex);
      intermediate = mult_v3f(&intermediate, bone_weight->weight);
      add_set_v3f(&skinned, &intermediate);

      intermediate_normal = mult_m4f_v3f(&inverse_transpose, &normal);
      intermediate_normal = mult_v3f(&intermediate_normal, bone_weight->weight);
      add_set_v3f(&skinned_normal, &intermediate_normal);

      total_weight += bone_weight->weight;
    }

    assert(IS_SAME_NP(total_weight, 1.f));

    vertices[idx + 0] = skinned.data[0];
    vertices[idx + 1] = skinned.data[1];
    vertices[idx + 2] = skinned.data[2];

    // NOTE: redundant, changes are minimal. But for correctness I kept it.
    normalize_set_v3f(&skinned_normal);

    normals[idx + 0] = skinned_normal.data[0];
    normals[idx + 1] = skinned_normal.data[1];
    normals[idx + 2] = skinned_normal.data[2];
  }
}

// NOTE: Since this is cpu only, root_inverse is not used right now, keeping it
// for later use
static
void
update_bone_transforms(
  anim_sequence_t *anim_sq,
  float anim_time,
  const matrix4f *root_inverse,
  skel_node_t *node,
  const matrix4f *parent_transform,
  const uint32_t index,
  const uint32_t root_motion)
{
  animation_t *anim = anim_sq->anim;
  skinned_mesh_t *skinned_mesh = anim_sq->skinned_mesh;
  skeleton_t *skeleton = &skinned_mesh->skeleton;
  bone_t *bone = node->bone_index == UINT32_MAX ?
    NULL : cvector_as(&skinned_mesh->bones, node->bone_index, bone_t);

  matrix4f global_transform;
  matrix4f *local = cvector_as(&anim_sq->local_transforms, index, matrix4f);

  // override local_transform if a valid channel exists
  anim_node_t *channel = find_anim_channel(anim_sq->anim, &node->name);
  matrix4f local_transform = node->transform;
  if (channel) {
    uint32_t index;
    matrix4f s_r_t[3];
    matrix4f_set_identity(s_r_t + 0);
    matrix4f_set_identity(s_r_t + 1);
    matrix4f_set_identity(s_r_t + 2);

    index = find_scale(channel, anim_time);
    if (index != UINT32_MAX) {
      scale_key_t *k_s = cvector_as(&channel->scale_keys, index++, scale_key_t);
      scale_key_t *k_e = cvector_as(&channel->scale_keys, index, scale_key_t);
      float lerp_factor = (anim_time - k_s->time) / (k_e->time - k_s->time);
      vector3f scale = lerp_v3f(k_s->value, k_e->value, lerp_factor);
      matrix4f_scale(s_r_t + 0, scale.data[0], scale.data[1], scale.data[2]);
    } else
      matrix4f_set_identity(s_r_t + 0);

    index = find_rotation(channel, anim_time);
    if (index != UINT32_MAX) {
      rotation_key_t *r_s = cvector_as(
        &channel->rotation_keys, index++, rotation_key_t);
      rotation_key_t *r_e = cvector_as(
        &channel->rotation_keys, index, rotation_key_t);
      float lerp_factor = (anim_time - r_s->time) / (r_e->time - r_s->time);
      quatf r = slerp_quatf(r_s->value, r_e->value, lerp_factor);
      s_r_t[1] = quatf_to_matrix4f(r);
    } else
      matrix4f_set_identity(s_r_t + 1);

    index = find_translation(channel, anim_time);
    if (index != UINT32_MAX) {
      position_key_t *p_s = cvector_as(
        &channel->position_keys, index++, position_key_t);
      position_key_t *p_e = cvector_as(
        &channel->position_keys, index, position_key_t);
      float lerp_factor = (anim_time - p_s->time) / (p_e->time - p_s->time);
      vector3f pos = lerp_v3f(p_s->value, p_e->value, lerp_factor);
      matrix4f_translation(s_r_t + 2, pos.data[0], pos.data[1], pos.data[2]);
    } else
      matrix4f_set_identity(s_r_t + 2);

    {
      matrix4f intermediate;
      intermediate = mult_m4f(s_r_t + 1, s_r_t + 0);
      local_transform = mult_m4f(s_r_t + 2, &intermediate);
    }
  }

  if (!root_motion) {
    local_transform.data[3] = node->transform.data[3];
    local_transform.data[7] = node->transform.data[7];
    local_transform.data[11] = node->transform.data[11];
  }

  global_transform = mult_m4f(parent_transform, &local_transform);
  *local = local_transform;

  if (bone) {
    matrix4f *final_transform = cvector_as(
      &anim_sq->final_transforms, node->bone_index, matrix4f);
    *final_transform = mult_m4f(&global_transform, &bone->offset_matrix);
  }

  for (uint32_t i = 0; i < node->skel_nodes.size; ++i) {
    uint32_t child_index = *cvector_as(&node->skel_nodes, i, uint32_t);
    skel_node_t *child = cvector_as(&skeleton->nodes, child_index, skel_node_t);
    update_bone_transforms(
      anim_sq,
      anim_time,
      root_inverse,
      child,
      &global_transform,
      child_index,
      root_motion);
  }
}

matrix4f
get_node_local_transform(anim_sequence_t *anim_sq, uint32_t index)
{
  return *cvector_as(&anim_sq->local_transforms, index, matrix4f);
}

float *
get_skin(anim_sequence_t *anim_sq)
{
  return (float *)anim_sq->vertices.data;
}

float *
get_skin_normals(anim_sequence_t *anim_sq)
{
  return (float *)anim_sq->normals.data;
}

void
reset_time(anim_sequence_t *anim_sq)
{
  anim_sq->time = 0.f;
}