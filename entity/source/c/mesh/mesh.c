/**
 * @file mesh.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2025-02-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <assert.h>
#include <string.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>
#include <library/allocator/allocator.h>
#include <math/c/common.h>
#include <math/c/matrix4f.h>
#include <entity/c/mesh/mesh.h>


void
mesh_def(void *ptr)
{
  assert(ptr);

  {
    mesh_t *mesh = (mesh_t *)ptr;
    memset(mesh, 0, sizeof(mesh_t));
  }
}

uint32_t 
mesh_is_def(const void *ptr)
{
  assert(ptr);

  {
    const mesh_t *mesh = (const mesh_t *)ptr;
    mesh_t def; 
    mesh_def(&def);
    return !memcmp(mesh, &def, sizeof(mesh_t));
  }
}

void 
mesh_serialize(
  const void *src, 
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const mesh_t *mesh = (const mesh_t *)src;
    size_t vtotal = sizeof(float) * 3 * mesh->vertices_count;
    size_t itotal = sizeof(uint32_t) * mesh->indices_count;
    size_t mtotal = sizeof(uint32_t) * MAX_MATERIAL_NUMBER;
    binary_stream_write(stream, &mesh->vertices_count, sizeof(uint32_t));
    binary_stream_write(stream, &mesh->indices_count, sizeof(uint32_t));
    binary_stream_write(stream, &mesh->materials.used, sizeof(uint32_t));
    binary_stream_write(stream, &mesh->materials.indices, mtotal);
    binary_stream_write(stream, &mesh->vertices, vtotal);
    binary_stream_write(stream, &mesh->normals, vtotal);
    binary_stream_write(stream, &mesh->uvs, vtotal);
    binary_stream_write(stream, &mesh->indices, itotal);
  }
}

void 
mesh_deserialize(
  void *dst, 
  const allocator_t *allocator, 
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    size_t mtotal = sizeof(uint32_t) * MAX_MATERIAL_NUMBER;
    mesh_t *mesh = (mesh_t *)dst;
    binary_stream_read(
      stream, (uint8_t *)&mesh->vertices_count, 
      sizeof(uint32_t), sizeof(uint32_t));
    binary_stream_read(
      stream, (uint8_t *)&mesh->indices_count, 
      sizeof(uint32_t), sizeof(uint32_t));
    binary_stream_read(
      stream, (uint8_t *)&mesh->materials.used, 
      sizeof(uint32_t), sizeof(uint32_t));
    binary_stream_read(
      stream, (uint8_t *)&mesh->materials.indices, 
      mtotal, mtotal);
    {
      size_t vtotal = sizeof(float) * 3 * mesh->vertices_count;
      size_t itotal = sizeof(uint32_t) * mesh->indices_count;
      mesh->vertices = (float *)allocator->mem_alloc(vtotal);
      mesh->normals = (float *)allocator->mem_alloc(vtotal);
      mesh->uvs = (float *)allocator->mem_alloc(vtotal);
      mesh->indices = (uint32_t *)allocator->mem_alloc(itotal);
      binary_stream_read(stream, (uint8_t *)&mesh->vertices, vtotal, vtotal);
      binary_stream_read(stream, (uint8_t *)&mesh->normals, vtotal, vtotal);
      binary_stream_read(stream, (uint8_t *)&mesh->uvs, vtotal, vtotal);
      binary_stream_read(stream, (uint8_t *)&mesh->indices, itotal, itotal);
    }
  }
}

size_t 
mesh_type_size(void)
{
  return sizeof(mesh_t);
}

uint32_t 
mesh_owns_alloc(void)
{
  return 0;
}

const allocator_t *
mesh_get_alloc(const void *ptr)
{
  return NULL;
}

void 
mesh_cleanup(
  void *ptr, 
  const allocator_t *allocator)
{
  assert(ptr && !mesh_is_def(ptr));
  assert(allocator);

  {
    mesh_t *mesh = (mesh_t *)ptr;
    allocator->mem_free(mesh->vertices);
    allocator->mem_free(mesh->normals);
    allocator->mem_free(mesh->uvs);
    allocator->mem_free(mesh->indices);
    memset(mesh, 0, sizeof(mesh_t));
  }
}

////////////////////////////////////////////////////////////////////////////////
void
mesh_setup(
  mesh_t *mesh,
  float *vertices,
  float *normals,
  float *uvs,
  uint32_t vertices_count,
  uint32_t *indices,
  uint32_t indices_count,
  material_array_t materials,
  const allocator_t* allocator)
{
  assert(allocator);
  assert(mesh && mesh_is_def(mesh));
  assert(vertices && normals && uvs && indices);

  {
    size_t vtotal = sizeof(float) * vertices_count * 3;
    size_t itotal = sizeof(uint32_t) * indices_count;
    mesh->vertices_count = vertices_count;
    mesh->indices_count = indices_count;
    mesh->materials = materials;
    mesh->vertices = (float*)allocator->mem_alloc(vtotal);
    memcpy(mesh->vertices, vertices, vtotal);
    mesh->normals = (float*)allocator->mem_alloc(vtotal);
    memcpy(mesh->normals, normals, vtotal);
    mesh->uvs = (float*)allocator->mem_alloc(vtotal);
    memcpy(mesh->uvs, uvs, vtotal);
    mesh->indices = (uint32_t*)allocator->mem_alloc(itotal);
    memcpy(mesh->indices, indices, itotal);
  }
}

static
void
create_unit_cube_face(
  float* vertices, 
  float* normals, 
  uint32_t* indices, 
  uint32_t start_index,
  matrix4f* transform)
{
  const float unit = 0.5f;
  uint32_t verti = 0;
  float normal[3];
  
  // transform set the normal.
  vector3f normalv = { 0, 0, 1.f };
  mult_set_m4f_v3f(transform, &normalv);
  normal[0] = normalv.data[0];
  normal[1] = normalv.data[1];
  normal[2] = normalv.data[2];

  vertices[verti * 3 + 0] = unit;
  vertices[verti * 3 + 1] = -unit;
  vertices[verti * 3 + 2] = unit;
  normals[verti * 3 + 0] = normal[0];
  normals[verti * 3 + 1] = normal[1];
  normals[verti * 3 + 2] = normal[2];
  
  ++verti;
  vertices[verti * 3 + 0] = -unit;
  vertices[verti * 3 + 1] = -unit;
  vertices[verti * 3 + 2] = unit;
  normals[verti * 3 + 0] = normal[0];
  normals[verti * 3 + 1] = normal[1];
  normals[verti * 3 + 2] = normal[2];
  
  ++verti; 
  vertices[verti * 3 + 0] = -unit;
  vertices[verti * 3 + 1] = unit;
  vertices[verti * 3 + 2] = unit;
  normals[verti * 3 + 0] = normal[0];
  normals[verti * 3 + 1] = normal[1];
  normals[verti * 3 + 2] = normal[2];
  
  ++verti;
  vertices[verti * 3 + 0] = unit;
  vertices[verti * 3 + 1] = unit;
  vertices[verti * 3 + 2] = unit;
  normals[verti * 3 + 0] = normal[0];
  normals[verti * 3 + 1] = normal[1];
  normals[verti * 3 + 2] = normal[2];
  
  // transform set the vertices.
  for (uint32_t i = 0; i < 4; ++i) {
    point3f vertex = 
      { vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2] };
    mult_set_m4f_p3f(transform, &vertex);
    vertices[i * 3 + 0] = vertex.data[0];
    vertices[i * 3 + 1] = vertex.data[1];
    vertices[i * 3 + 2] = vertex.data[2];
  }

  {
    indices[0] = start_index + 0;
    indices[1] = start_index + 3;
    indices[2] = start_index + 1;

    indices[3] = start_index + 1;
    indices[4] = start_index + 3;
    indices[5] = start_index + 2;
  }
}

mesh_t*
create_unit_cube(const allocator_t* allocator)
{
  uint32_t vertices_count = 24;
  uint32_t faces_count = vertices_count / 2;
  uint32_t indices_count = faces_count * 3;
  mesh_t* mesh = (mesh_t*)allocator->mem_alloc(sizeof(mesh_t));  
  assert(mesh != NULL);
  memset(mesh, 0, sizeof(mesh_t));

  mesh->vertices = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  mesh->vertices_count = vertices_count;

  mesh->normals = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  mesh->uvs = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  memset(mesh->uvs, 0, sizeof(float) * 3 * vertices_count);

  mesh->indices = (uint32_t*)allocator->mem_alloc(
    sizeof(uint32_t) * indices_count);
  mesh->indices_count = indices_count;

  {
    float* vertices = mesh->vertices;
    float* normals = mesh->normals;
    float* uvs = mesh->uvs;
    uint32_t* indices = mesh->indices;
    matrix4f transform[6];
    matrix4f_set_identity(&transform[0]);
    matrix4f_rotation_y(&transform[1], K_PI/2.f);
    matrix4f_rotation_y(&transform[2], K_PI);
    matrix4f_rotation_y(&transform[3], 3.f*K_PI/2.f);
    matrix4f_rotation_x(&transform[4], K_PI/2.f);
    matrix4f_rotation_x(&transform[5], 3.f*K_PI/2.f);

    for (uint32_t facei = 0; facei < 6; ++facei) {
      uint32_t base_offset = facei * 4 * 3;
      uint32_t index_offset = facei * 6;
      uint32_t start_index = facei * 4;
      create_unit_cube_face(
        vertices + base_offset, 
        normals + base_offset, 
        indices + index_offset, 
        start_index, 
        transform + facei);
    }
  }

  return mesh;
}

mesh_t*
create_unit_sphere(const int32_t factor, const allocator_t* allocator)
{
  const double half_pi = K_PI/2.;
  const double height_angle_increment = K_PI/(factor + 1);
  double ring_increment = half_pi/factor;
  uint32_t vertices_count = factor * factor * 4 + 2;
  uint32_t faces_count = factor * 4 * 2 + (factor - 1) * (factor * 4 * 2); 
  uint32_t indices_count = faces_count * 3;

  mesh_t* mesh = (mesh_t*)allocator->mem_alloc(sizeof(mesh_t));  
  assert(mesh != NULL);
  assert(factor >= 1);
  memset(mesh, 0, sizeof(mesh_t));

  mesh->vertices = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  mesh->vertices_count = vertices_count;

  mesh->normals = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  mesh->uvs = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  memset(mesh->uvs, 0, sizeof(float) * 3 * vertices_count);

  mesh->indices = (uint32_t*)allocator->mem_alloc(
    sizeof(uint32_t) * indices_count);
  mesh->indices_count = indices_count;

  // push the top and bottom vertices (0, 1, 0), (0, -1, 0) respectively.
  mesh->vertices[0 * 3 + 0] = 0.f;
  mesh->vertices[0 * 3 + 1] = 1.f;
  mesh->vertices[0 * 3 + 2] = 0.f;
  mesh->normals[0 * 3 + 0] = 0.f;
  mesh->normals[0 * 3 + 1] = 1.f;
  mesh->normals[0 * 3 + 2] = 0.f;

  mesh->vertices[(vertices_count - 1) * 3 + 0] = 0.f;
  mesh->vertices[(vertices_count - 1) * 3 + 1] = -1.f;
  mesh->vertices[(vertices_count - 1) * 3 + 2] = 0.f;
  mesh->normals[(vertices_count - 1) * 3 + 0] = 0.f;
  mesh->normals[(vertices_count - 1) * 3 + 1] = -1.f;
  mesh->normals[(vertices_count - 1) * 3 + 2] = 0.f;

  for (int32_t level = 1, current_vertex = 1; level <= factor; ++level) {
    double height_angle_total = height_angle_increment * level;
    double height = cos(height_angle_total);        // y
    double radius = sin(height_angle_total);

    for (
      int32_t ring_vertex = 0, total = factor * 4; 
      ring_vertex < total; 
      ++ring_vertex) {
      double ring_angle = ring_vertex * ring_increment;
      double horizontal_offset = cos(ring_angle) * radius;   // x
      double depth_offset = sin(ring_angle) * radius;        // z

      mesh->vertices[current_vertex * 3 + 0] = horizontal_offset;
      mesh->vertices[current_vertex * 3 + 1] = height;
      mesh->vertices[current_vertex * 3 + 2] = depth_offset;
      mesh->normals[current_vertex * 3 + 0] = horizontal_offset;
      mesh->normals[current_vertex * 3 + 1] = height;
      mesh->normals[current_vertex * 3 + 2] = depth_offset;
      ++current_vertex;
    }
  }

  // build the indices.
  {
    int32_t current_face = 0;
    int32_t starting_vertex = 1;
    int32_t faces_per_ring = factor * 4;
    // build the top ribbon indices.
    for (
      int32_t i = 0; 
      i < faces_per_ring; 
      ++starting_vertex, ++current_face, ++i) {
      mesh->indices[current_face * 3 + 2] = 0;
      mesh->indices[current_face * 3 + 1] = starting_vertex + 0;
      mesh->indices[current_face * 3 + 0] = 
        starting_vertex + 1 - ((i == faces_per_ring - 1) ? faces_per_ring : 0);
    }

    // build the intermediate ribbons indices.
    for (int32_t level = 1; level < factor; ++level) {
      for (
        int32_t i = 0, starting_vertex = 1 + (level - 1) * faces_per_ring; 
        i < faces_per_ring; 
        ++starting_vertex, ++i) {
        int32_t next_vertex = 
          starting_vertex + 1 - 
          ((i == faces_per_ring - 1) ? faces_per_ring : 0);

        mesh->indices[current_face * 3 + 2] = starting_vertex;
        mesh->indices[current_face * 3 + 1] = starting_vertex + faces_per_ring;
        mesh->indices[current_face * 3 + 0] = next_vertex;
        ++current_face;

        mesh->indices[current_face * 3 + 2] = next_vertex;
        mesh->indices[current_face * 3 + 1] = starting_vertex + faces_per_ring;
        mesh->indices[current_face * 3 + 0] = next_vertex + faces_per_ring;
        ++current_face;
      }
    }

    // build the bottom ribbon indices.
    starting_vertex = vertices_count - faces_per_ring - 1;
    for (
      int32_t i = 0; 
      i < faces_per_ring; 
      ++starting_vertex, ++current_face, ++i) {
      mesh->indices[current_face * 3 + 2] = starting_vertex + 0;
      mesh->indices[current_face * 3 + 1] = vertices_count - 1;
      mesh->indices[current_face * 3 + 0] = 
        starting_vertex + 1 - ((i == faces_per_ring - 1) ? faces_per_ring : 0);
    }
  }

  return mesh;
}

mesh_t*
create_unit_capsule(
  const int32_t factor, 
  const float half_height_to_radius_ratio,
  const allocator_t* allocator)
{
  const double half_pi = K_PI / 2.;
  const double height_angle_increment = half_pi / factor;
  double ring_increment = half_pi / factor;
  uint32_t vertices_ring_count = factor * 4;
  uint32_t total_ring_count = factor * 2; // exluding the tip vertices.
  const uint32_t tip_vertices_count = 2;
  uint32_t vertices_count = 
    total_ring_count * vertices_ring_count + tip_vertices_count;
  uint32_t fan_faces_count = factor * 4;
  uint32_t ribbon_face_count = factor * 4 * 2;
  uint32_t faces_count = 
    fan_faces_count * 2 + 
    (factor - 1) * 2 * ribbon_face_count + 
    ribbon_face_count;
  uint32_t indices_count = faces_count * 3;
  float total_sections = half_height_to_radius_ratio + 1.f;
  float height_to_add = (1.f / total_sections) * half_height_to_radius_ratio;
  float radius_ratio = height_to_add * 1.f / half_height_to_radius_ratio;

  mesh_t* mesh = (mesh_t*)allocator->mem_alloc(sizeof(mesh_t));
  assert(mesh != NULL);
  assert(half_height_to_radius_ratio > 0.f);
  assert(factor >= 1);
  memset(mesh, 0, sizeof(mesh_t));

  mesh->materials.used = 0;

  {
    mesh->vertices = NULL;
    mesh->normals = NULL;
    mesh->uvs = NULL;
    mesh->indices = NULL;
  }

  mesh->vertices = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  mesh->vertices_count = vertices_count;

  mesh->normals = (float*)allocator->mem_alloc(
    sizeof(float) * 3 * vertices_count);
  mesh->uvs = (float*)allocator->mem_alloc(sizeof(float) * 3 * vertices_count);
  memset(mesh->uvs, 0, sizeof(float) * 3 * vertices_count);

  mesh->indices = (uint32_t*)allocator->mem_alloc(
    sizeof(uint32_t) * indices_count);
  mesh->indices_count = indices_count;

  // push the top and bottom vertices (0, 1, 0), (0, -1, 0) respectively.
  mesh->vertices[0 * 3 + 0] = 0.f;
  mesh->vertices[0 * 3 + 1] = 1.f;// + height_to_add;
  mesh->vertices[0 * 3 + 2] = 0.f;
  mesh->normals[0 * 3 + 0] = 0.f;
  mesh->normals[0 * 3 + 1] = 1.f;
  mesh->normals[0 * 3 + 2] = 0.f;

  mesh->vertices[(vertices_count - 1) * 3 + 0] = 0.f;
  mesh->vertices[(vertices_count - 1) * 3 + 1] = -1.f;// - height_to_add;
  mesh->vertices[(vertices_count - 1) * 3 + 2] = 0.f;
  mesh->normals[(vertices_count - 1) * 3 + 0] = 0.f;
  mesh->normals[(vertices_count - 1) * 3 + 1] = -1.f;
  mesh->normals[(vertices_count - 1) * 3 + 2] = 0.f;

  // create the vertices, in case of a mesh we do this in 2 halfs, we also
  // need to duplicate the middle ring's vertices.
  {
    int32_t current_vertex = 1;
    // top half of the mesh.
    for (int32_t level = 1; level <= factor; ++level) {
      double height_angle_total = height_angle_increment * level;
      double height = cos(height_angle_total) * radius_ratio;        // y
      double radius = sin(height_angle_total) * radius_ratio;

      for (
        int32_t ring_vertex = 0, total = factor * 4; 
        ring_vertex < total; 
        ++ring_vertex) {
        double ring_angle = ring_vertex * ring_increment;
        double horizontal_offset = cos(ring_angle) * radius;   // x
        double depth_offset = sin(ring_angle) * radius;        // z

        mesh->vertices[current_vertex * 3 + 0] = horizontal_offset;
        mesh->vertices[current_vertex * 3 + 1] = height + height_to_add;
        mesh->vertices[current_vertex * 3 + 2] = depth_offset;
        mesh->normals[current_vertex * 3 + 0] = horizontal_offset;
        mesh->normals[current_vertex * 3 + 1] = height;
        mesh->normals[current_vertex * 3 + 2] = depth_offset;
        ++current_vertex;
      }
    }

    // bottom half of the capule.
    for (int32_t level = 0; level < factor; ++level) {
      double height_angle_total = half_pi + height_angle_increment * level;
      double height = cos(height_angle_total) * radius_ratio;        // y
      double radius = sin(height_angle_total) * radius_ratio;

      for (
        int32_t ring_vertex = 0, total = factor * 4; 
        ring_vertex < total; 
        ++ring_vertex) {
        double ring_angle = ring_vertex * ring_increment;
        double horizontal_offset = cos(ring_angle) * radius;   // x
        double depth_offset = sin(ring_angle) * radius;        // z

        mesh->vertices[current_vertex * 3 + 0] = horizontal_offset;
        mesh->vertices[current_vertex * 3 + 1] = height - height_to_add;
        mesh->vertices[current_vertex * 3 + 2] = depth_offset;
        mesh->normals[current_vertex * 3 + 0] = horizontal_offset;
        mesh->normals[current_vertex * 3 + 1] = height;
        mesh->normals[current_vertex * 3 + 2] = depth_offset;
        ++current_vertex;
      }
    }
  }

  // build the indices.
  {
    int32_t current_face = 0;
    int32_t starting_vertex = 1;
    int32_t faces_per_ring = factor * 4;
    // build the top ribbon indices.
    for (
      int32_t i = 0; 
      i < faces_per_ring; 
      ++starting_vertex, ++current_face, ++i) {
      mesh->indices[current_face * 3 + 2] = 0;
      mesh->indices[current_face * 3 + 1] = starting_vertex + 0;
      mesh->indices[current_face * 3 + 0] = 
        starting_vertex + 1 - ((i == faces_per_ring - 1) ? faces_per_ring : 0);
    }

    // build the intermediate ribbons indices.
    for (
      int32_t level = 0, count = (factor - 1) * 2 + 1; 
      level < count; 
      ++level) {
      for (
        int32_t i = 0, starting_vertex = 1 + level * faces_per_ring; 
        i < faces_per_ring; 
        ++starting_vertex, ++i) {
        int32_t next_vertex = 
          starting_vertex + 1 - 
          ((i == faces_per_ring - 1) ? faces_per_ring : 0);

        mesh->indices[current_face * 3 + 2] = starting_vertex;
        mesh->indices[current_face * 3 + 1] = starting_vertex + faces_per_ring;
        mesh->indices[current_face * 3 + 0] = next_vertex;
        ++current_face;

        mesh->indices[current_face * 3 + 2] = next_vertex;
        mesh->indices[current_face * 3 + 1] = starting_vertex + faces_per_ring;
        mesh->indices[current_face * 3 + 0] = next_vertex + faces_per_ring;
        ++current_face;
      }
    }

    // build the bottom ribbon indices.
    starting_vertex = vertices_count - faces_per_ring - 1;
    for (
      int32_t i = 0; 
      i < faces_per_ring; 
      ++starting_vertex, ++current_face, ++i) {
      mesh->indices[current_face * 3 + 2] = starting_vertex + 0;
      mesh->indices[current_face * 3 + 1] = vertices_count - 1;
      mesh->indices[current_face * 3 + 0] = starting_vertex + 1 - 
        ((i == faces_per_ring - 1) ? faces_per_ring : 0);
    }
  }

  return mesh;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_mesh_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = mesh_def;
  vtable.fn_is_def = mesh_is_def;
  vtable.fn_serialize = mesh_serialize;
  vtable.fn_deserialize = mesh_deserialize;
  vtable.fn_type_size = mesh_type_size;
  vtable.fn_owns_alloc = mesh_owns_alloc;
  vtable.fn_get_alloc = mesh_get_alloc;
  vtable.fn_cleanup = mesh_cleanup;
  register_type(get_type_id(mesh_t), &vtable);
}