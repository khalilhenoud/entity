/**
 * @file animation_utils.h
 * @author khalilhenoud@gmail.com
 * @brief tmp: functionality for playing the animation (proof of concept)
 * @version 0.1
 * @date 2026-01-15
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef SCENE_ANIMATION_UTILS_H
#define SCENE_ANIMATION_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <entity/internal/module.h>


// NOTE: intended as a proof of concept that the animation code works
typedef struct allocator_t allocator_t;
typedef struct mesh_t mesh_t;
typedef struct skinned_mesh_t skinned_mesh_t;
typedef struct animation_t animation_t;
typedef struct anim_sequence_t anim_sequence_t;

ENTITY_API
anim_sequence_t *
play_anim(
  animation_t *anim,
  skinned_mesh_t *skinned_mesh,
  const allocator_t *allocator);

// anim_sq is no longer valid after this...
ENTITY_API
void
stop_anim(anim_sequence_t *anim_sq);

ENTITY_API
void
update_anim(anim_sequence_t *anim_sq, float delta_t);

ENTITY_API
matrix4f
get_anim_bone_transform(anim_sequence_t *anim_sq, uint32_t index);

#ifdef __cplusplus
}
#endif

#endif