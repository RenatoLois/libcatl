#include "catl/arena/arena.h"
#include "catl/catl_internal/catl_exit_status.h"
#include <stdlib.h>
#include <stdint.h>

CatlExitStatus catl_arena_create(catl_arena_t** out, size_t data_size, size_t length) {
  if(length != 0 && data_size > SIZE_MAX / length) return NULL;
  catl_arena_t* arena = malloc(sizeof(catl_arena_t));
  if(!arena) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  arena->capacity = length;
  arena->buffer = malloc(data_size * arena->capacity);
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

CatlExitStatus catl_arena_alloc(void** out, catl_arena_t* arena, size_t data_size, size_t length) {
  if(!arena) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  if(data_size == 0) return CATL_STATUS_INVALID_VALUE;

  // lenght * data_size results perhaps > MAX_FLOAT, so divides by data_size
  // in right side instead multiplicating on the left side
  if(length > (arena->capacity - arena->offset) / data_size) return CATL_STATUS_LIMIT_EXCEEDED;

  void* ptr = (unsigned char*) arena->buffer + arena->offset;

  arena->offset += length * data_size;

  *out = ptr;
}
