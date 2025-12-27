cmake_minimum_required(VERSION 3.22)

# set the project name
project(entity VERSION 1.0)

# add the executable
add_library(${PROJECT_NAME} SHARED
        ./source/mesh/mesh_utils.c
        ./source/mesh/mesh.c
        ./source/mesh/material.c
        ./source/mesh/texture.c
        ./source/mesh/skinned_mesh.c
        ./source/misc/font.c
        ./source/runtime/font_utils.c
        ./source/runtime/texture_utils.c
        ./source/spatial/bvh.c
        ./source/scene/camera.c
        ./source/scene/light.c
        ./source/scene/node.c
        ./source/scene/scene.c
        ./include/entity/internal/module.h
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC library
        PUBLIC math)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        )