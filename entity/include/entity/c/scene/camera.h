/**
 * @file camera.h
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H

#include <math/c/vector3f.h>


typedef
struct camera_t {
  vector3f position;
  vector3f lookat_direction;
  vector3f up_vector;
} camera_t;

#endif