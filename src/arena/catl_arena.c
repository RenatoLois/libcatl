#include "catl/arena/catl_arena.h"
#include "catl/catl_internal/catl_exit_status.h"
#include <stdlib.h>
#include <stdint.h>

CatlExitStatus catl_arena_create(catl_arena_t** out, const size_t total_length) {
  catl_arena_t* arena = malloc(sizeof(catl_arena_t));
  if(!arena) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  arena->total_length = total_length;
  arena->buffer = malloc(arena->total_length);
  if(!arena->buffer) {
    free(arena);
    return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  }
  arena->offset=0;
  
  *out = arena;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_arena_destroy(catl_arena_t* arena) {
  if(!arena) return CATL_STATUS_NULL_POINTER;
  if(arena->buffer) free(arena->buffer);
  free(arena);
  return CATL_STATUS_OK;
}

CatlExitStatus catl_arena_alloc(void** out, catl_arena_t* arena, const size_t length) {
  if(!arena) return CATL_STATUS_NULL_POINTER;

  // prevents integer overflow in multiplication by dividing
  // on the right side instead of multiplying on the left
  if(length > (arena->total_length - arena->offset)) return CATL_STATUS_LIMIT_EXCEEDED;

  void* ptr = (unsigned char*) arena->buffer + arena->offset;

  arena->offset += length;

  *out = ptr;

  return CATL_STATUS_OK;
}
