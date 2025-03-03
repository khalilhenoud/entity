cmake_minimum_required(VERSION 3.22)

# set the project name
project(entity VERSION 1.0)

# TO_DELETE
#   font_utils.h/.c
#   light_utils.h/.c
#   texture_utils.h/.c
#   material_utils.h/.c
#   mesh_utils.h/.c; part of the files needs trimming.
#   camera_utils.h/.c
#   node_utils.h/.c
#   bvh_utils.h/.c
#   scene_utils.h/.c

# add the executable
add_library(${PROJECT_NAME} SHARED
        ./source/c/mesh/mesh_utils.c
        ./source/c/mesh/mesh.c
        ./source/c/mesh/material_utils.c
        ./source/c/mesh/material.c
        ./source/c/mesh/texture_utils.c
        ./source/c/mesh/texture.c
        ./source/c/misc/font_utils.c
        ./source/c/misc/font.c
        ./source/c/runtime/font_utils.c
        ./source/c/runtime/texture_utils.c
        ./source/c/spatial/bvh_utils.c
        ./source/c/spatial/bvh.c
        ./source/c/scene/camera.c
        ./source/c/scene/camera_utils.c
        ./source/c/scene/light_utils.c
        ./source/c/scene/light.c
        ./source/c/scene/node.c
        ./source/c/scene/node_utils.c
        ./source/c/scene/scene_utils.c
        ./source/c/scene/scene.c
        ./include/entity/c/internal/module.h
        )

target_link_libraries(${PROJECT_NAME} 
        PUBLIC library
        PUBLIC math)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        )