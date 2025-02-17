/**
 * @file node_utils.c
 * @author khalilhenoud@gmail.com
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <assert.h>
#include <library/allocator/allocator.h>
#include <library/string/cstring.h>
#include <entity/c/scene/node.h>
#include <entity/c/scene/node_utils.h>


node_t*
allocate_node_array(uint32_t count, const allocator_t* allocator)
{
  assert(count && allocator);

  {
    node_t* ptr = (node_t*)allocator->mem_cont_alloc(count, sizeof(node_t));
    assert(ptr && "Failed to allocate memory");
    return ptr;
  }
}

void
free_node_array(
  node_t* node, 
  uint32_t count, 
  const allocator_t* allocator)
{
  assert(node && count && allocator);

  for (uint32_t i = 0; i < count; ++i)
    free_node_internal(node + i, allocator);

  allocator->mem_free(node);
}

void
free_node_internal(node_t* node, const allocator_t* allocator)
{
  assert(node && allocator);

  cstring_cleanup(node->name, NULL);
  allocator->mem_free(node->name);

  if (node->meshes.count) {
    assert(node->meshes.indices);
    allocator->mem_free(node->meshes.indices);
  }

  if (node->nodes.count) {
    assert(node->nodes.indices);
    allocator->mem_free(node->nodes.indices);
  }
}

void
free_node(node_t* node, const allocator_t* allocator)
{
  assert(node && allocator);

  free_node_internal(node, allocator);
  allocator->mem_free(node);  
}