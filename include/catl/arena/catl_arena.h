#ifndef CATL_ARENA_H
#define CATL_ARENA_H

#include "catl/catl_internal/catl_exit_status.h"
#include <stddef.h>

typedef struct catl_arena_t {
  void* buffer;
  size_t total_length;
  size_t offset;
} catl_arena_t;

CatlExitStatus catl_arena_create(catl_arena_t** out, const size_t total_length);

CatlExitStatus catl_arena_destroy(catl_arena_t* arena);

CatlExitStatus catl_arena_alloc(void** out, catl_arena_t* arena, const size_t length);

#endif
