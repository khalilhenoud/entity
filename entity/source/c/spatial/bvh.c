/**
 * @file bvh.c
 * @author khalilhenoud@gmail.com
 * @brief non-general bvh for simplistic collision detection.
 * @version 0.1
 * @date 2023-10-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <math/c/face.h>
#include <entity/c/spatial/bvh.h>
#include <library/allocator/allocator.h>

#define BVH_PRIMITIVES_PER_LEAF 8


uint32_t
get_bvh_primitives_per_leaf(void)
{
  return BVH_PRIMITIVES_PER_LEAF;
}

void
merge_aabb(
  bvh_aabb_t* dst, 
  const bvh_aabb_t* a, 
  const bvh_aabb_t* b)
{
  dst->min_max[0].data[0] = fmin(a->min_max[0].data[0], b->min_max[0].data[0]);
  dst->min_max[0].data[1] = fmin(a->min_max[0].data[1], b->min_max[0].data[1]);
  dst->min_max[0].data[2] = fmin(a->min_max[0].data[2], b->min_max[0].data[2]);

  dst->min_max[1].data[0] = fmax(a->min_max[1].data[0], b->min_max[1].data[0]);
  dst->min_max[1].data[1] = fmax(a->min_max[1].data[1], b->min_max[1].data[1]);
  dst->min_max[1].data[2] = fmax(a->min_max[1].data[2], b->min_max[1].data[2]);
}

void
merge_aabb_inplace(
  bvh_aabb_t* dst, 
  const bvh_aabb_t* b)
{
  bvh_aabb_t a = *dst;
  dst->min_max[0].data[0] = fmin(a.min_max[0].data[0], b->min_max[0].data[0]);
  dst->min_max[0].data[1] = fmin(a.min_max[0].data[1], b->min_max[0].data[1]);
  dst->min_max[0].data[2] = fmin(a.min_max[0].data[2], b->min_max[0].data[2]);

  dst->min_max[1].data[0] = fmax(a.min_max[1].data[0], b->min_max[1].data[0]);
  dst->min_max[1].data[1] = fmax(a.min_max[1].data[1], b->min_max[1].data[1]);
  dst->min_max[1].data[2] = fmax(a.min_max[1].data[2], b->min_max[1].data[2]);
}

static
void
get_aabb(point3f points[3], bvh_aabb_t* aabb)
{
  point3f* min = aabb->min_max + 0;
  point3f* max = aabb->min_max + 1;
  *min = points[0];
  *max = points[0];

  for (uint32_t i = 0; i < 3; ++i) {
    min->data[0] = fmin(min->data[0], points[i].data[0]);
    min->data[1] = fmin(min->data[1], points[i].data[1]);
    min->data[2] = fmin(min->data[2], points[i].data[2]);

    max->data[0] = fmax(max->data[0], points[i].data[0]);
    max->data[1] = fmax(max->data[1], points[i].data[1]);
    max->data[2] = fmax(max->data[2], points[i].data[2]);
  }
}

static
void
get_centroid(bvh_aabb_t* aabb, point3f* centroid)
{
  vector3f diff = diff_v3f(aabb->min_max + 0, aabb->min_max + 1);
  mult_set_v3f(&diff, 0.5f);
  *centroid = add_v3f(aabb->min_max, &diff);
}

static
void
swap_faces(bvh_t* bvh, uint32_t left, uint32_t right)
{
  assert(bvh);
  assert(left < bvh->count && right < bvh->count);

  // swap the faces, the normals, and the bounds.
  {
    face_t copy = bvh->faces[left];
    bvh->faces[left] = bvh->faces[right];
    bvh->faces[right] = copy;
  }

  {
    vector3f normal = bvh->normals[left];
    bvh->normals[left] = bvh->normals[right];
    bvh->normals[right] = normal;
  }

  {
    bvh_aabb_t bounds = bvh->bounds[left];
    bvh->bounds[left] = bvh->bounds[right];
    bvh->bounds[right] = bounds;
  }
}

static
void
get_subdivision_plane_naive(
  bvh_t* bvh,
  bvh_node_t* node, 
  uint32_t first_prim,
  uint32_t last_prim_excl,
  point3f* center, 
  uint32_t* axis)
{
  int32_t left[] = { 0, 0, 0 }, right[] = { 0, 0, 0 };
  int32_t quota = INT32_MAX;
  point3f face_centroid;
  get_centroid(&node->bounds, center);

  for (uint32_t j = first_prim; j < last_prim_excl; ++j) {
    get_centroid(&bvh->bounds[j], &face_centroid);

    for (uint32_t candidate = 0; candidate < 3; ++candidate) {
      if (face_centroid.data[candidate] < center->data[candidate])
        ++left[candidate];
      else
        ++right[candidate];
    }
  }

  for (uint32_t candidate = 0; candidate < 3; ++candidate) {
    uint32_t new_quota = abs(right[candidate] - left[candidate]);
    if (new_quota < quota) {
      quota = new_quota;
      *axis = candidate;
    }
  }

  assert(quota != INT32_MAX);
}

static 
void
subdivide_naive(
  bvh_t* bvh, 
  uint32_t first_prim, 
  uint32_t last_prim_excl, 
  uint32_t index)
{
  assert(bvh);
  assert(bvh->nodes_used < get_max_node_count(bvh->count));
  assert(index < get_max_node_count(bvh->count));

  {
    bvh_node_t* node = bvh->nodes + index;
    point3f center;
    uint32_t axis;
    uint32_t i_prims[3] = {0, 0, 0};

    // calculate the node aabb.
    node->bounds = bvh->bounds[first_prim];
    for (uint32_t i = first_prim; i < last_prim_excl; ++i)
      merge_aabb_inplace(&node->bounds, &bvh->bounds[i]);

    // stop the recursion if we have reached our goal.
    if ((last_prim_excl - first_prim) <= get_bvh_primitives_per_leaf()) {
      node->left_first = first_prim;
      node->tri_count = last_prim_excl - first_prim;
      return;
    }
      
    get_subdivision_plane_naive(
      bvh, node, first_prim, last_prim_excl, &center, &axis);

    {
      // split the list of primitives.
      uint32_t i = first_prim;
      uint32_t j = last_prim_excl;
      point3f centroid;

      while (i < j) {
        get_centroid(bvh->bounds + i, &centroid);
        if (centroid.data[axis] < center.data[axis])
          ++i;
        else
          swap_faces(bvh, i, --j);
      }

      // another reason to stop the recursion would be that no faces has been
      // split
      if (i == first_prim || j == last_prim_excl) {
        node->left_first = first_prim;
        node->tri_count = last_prim_excl - first_prim;
        return;
      }
      
      // we are dealing with an interior node.
      node->tri_count = 0;
      node->left_first = bvh->nodes_used;
      bvh->nodes_used += 2;

      subdivide_naive(bvh, first_prim, i, node->left_first + 0);
      subdivide_naive(bvh, i, last_prim_excl, node->left_first + 1);
    }
  }
}

static
void
construct_bvh_naive(bvh_t* bvh, const allocator_t* allocator)
{
  bvh_node_t* root = bvh->nodes;
  root->left_first = root->tri_count = 0;
  // this will allow us to skip the first node after the root, ensuring the left
  // and right nodes are on the same cacheline once aligned.
  bvh->nodes_used += 2;
  subdivide_naive(bvh, 0, bvh->count, 0);
}

bvh_t*
create_bvh(
  float** vertices, 
  uint32_t** indices, 
  uint32_t* indices_count, 
  uint32_t multi_count,
  const allocator_t* allocator,
  bvh_construction_method_t method)
{
  assert(vertices && indices && indices_count && multi_count && allocator);
  assert(method != BVH_CONSTRUCT_COUNT);

  // ensure the data is valid.
  for (uint32_t i = 0; i < multi_count; ++i)
    assert(vertices[i] && indices[i] && indices_count[i]);

  {
    bvh_t* bvh = allocator->mem_alloc(sizeof(bvh_t));
    assert(bvh);
    memset(bvh, 0, sizeof(bvh_t));

    // find the total number of faces to be added.
    for (uint32_t i = 0; i < multi_count; ++i)
      bvh->count += indices_count[i]/3;

    bvh->faces = allocator->mem_alloc(bvh->count * sizeof(face_t));
    bvh->normals = allocator->mem_alloc(bvh->count * sizeof(vector3f));
    bvh->bounds = allocator->mem_alloc(bvh->count * sizeof(bvh_aabb_t));

    {
      uint32_t iarray[3], count;
      face_t* face;
      vector3f* normal;
      bvh_aabb_t* aabb;
      uint32_t face_index = 0;
      for (uint32_t mesh_index = 0; mesh_index < multi_count; ++mesh_index) {
        count = indices_count[mesh_index];

        for (uint32_t i = 0; i < count; i+=3) {
          iarray[0] = indices[mesh_index][i + 0];
          iarray[1] = indices[mesh_index][i + 1];
          iarray[2] = indices[mesh_index][i + 2];

          face = bvh->faces + face_index;
          normal = bvh->normals + face_index;
          aabb = bvh->bounds + face_index;

          // set the face vertices.
          for (uint32_t k = 0; k < 3; ++k) {
            face->points[k].data[0] = vertices[mesh_index][iarray[k] * 3 + 0];
            face->points[k].data[1] = vertices[mesh_index][iarray[k] * 3 + 1];
            face->points[k].data[2] = vertices[mesh_index][iarray[k] * 3 + 2];
          }

          {
            // calculate bounds
            get_aabb(face->points, aabb);
          }

          {
            // calculate normal
            vector3f v1, v2;
            vector3f_set_diff_v3f(&v1, face->points, face->points + 1);
            vector3f_set_diff_v3f(&v2, face->points, face->points + 2);
            *normal = cross_product_v3f(&v1, &v2);
            normalize_set_v3f(normal);
          }

          {
            // if the face is not valid, we swap it with the last face and
            // decrease the count in the bvh. we also do not increase the
            // face_index (by continuing).
            uint32_t is_valid = !(
              equal_to_v3f(face->points + 0, face->points + 1) ||
              equal_to_v3f(face->points + 0, face->points + 2) ||
              equal_to_v3f(face->points + 1, face->points + 2));
            if (!is_valid) {
              swap_faces(bvh, face_index, bvh->count - 1);
              --bvh->count;
              continue;
            }
          }

          ++face_index;
        }
      }
    }

    {
      uint32_t nodes_count = get_max_node_count(bvh->count);
      bvh->nodes = allocator->mem_alloc(nodes_count * sizeof(bvh_node_t));
      memset(bvh->nodes, 0, sizeof(bvh_node_t) * nodes_count);
    }

    if (method == BVH_CONSTRUCT_NAIVE)
      construct_bvh_naive(bvh, allocator);
    else
      assert(0);

    return bvh;
  }
}

void
free_bvh(bvh_t* bvh, const allocator_t* allocator)
{
  assert(bvh && allocator);

  // we might have moved the pointers to avoid duplicates.
  if (bvh->faces)
    allocator->mem_free(bvh->faces);
  if (bvh->normals)
    allocator->mem_free(bvh->normals);
  if (bvh->bounds)
    allocator->mem_free(bvh->bounds);
  if (bvh->nodes)
    allocator->mem_free(bvh->nodes);

  allocator->mem_free(bvh);
}

int32_t
bounds_intersect(const bvh_aabb_t* left, const bvh_aabb_t* right)
{
  int32_t no_intersect_x = 
    left->min_max[0].data[0] > right->min_max[1].data[0] ||
    left->min_max[1].data[0] < right->min_max[0].data[0];
  int32_t no_intersect_y = 
    left->min_max[0].data[1] > right->min_max[1].data[1] ||
    left->min_max[1].data[1] < right->min_max[0].data[1];
  int32_t no_intersect_z = 
    left->min_max[0].data[2] > right->min_max[1].data[2] ||
    left->min_max[1].data[2] < right->min_max[0].data[2];
  return !(no_intersect_x || no_intersect_y || no_intersect_z);
}

static
int32_t
is_leaf(const bvh_node_t* node)
{
  return node->tri_count != 0;
}

static
void
query_intersection_fixed_256_internal(
  bvh_t* bvh, 
  uint32_t node_index,
  bvh_aabb_t* bounds, 
  uint32_t array[256], 
  uint32_t* used)
{
  bvh_node_t* node = bvh->nodes + node_index;
  if (is_leaf(node)) {
    if (bounds_intersect(&node->bounds, bounds))
      array[(*used)++] = node_index;
    return;
  }

  if (!bounds_intersect(&node->bounds, bounds))
    return;
  
  query_intersection_fixed_256_internal(
    bvh, node->left_first + 0, bounds, array, used);
  query_intersection_fixed_256_internal(
    bvh, node->left_first + 1, bounds, array, used);
}

void
query_intersection_fixed_256(
  bvh_t* bvh, 
  bvh_aabb_t* bounds, 
  uint32_t array[256], 
  uint32_t* used)
{
  assert(bvh && bounds && array && used);
  *used = 0;

  query_intersection_fixed_256_internal(bvh, 0, bounds, array, used);
}