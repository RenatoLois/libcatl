#ifndef CATL_STATIC_ARRAY_H
#define CATL_STATIC_ARRAY_H

#include "catl/catl_internal/catl_exit_status.h"
#include <stddef.h>
#include <stdbool.h>

#define CATL_STATIC_ARRAY_DEFAULT_CAPACITY 8

typedef struct {
  void* array;
  size_t capacity;
  size_t data_size;
  void (* delete_function )(void*);
} catl_static_array_t;

CatlExitStatus catl_array_create(catl_static_array_t** out, const size_t data_size, const size_t capacity, void (* delete_function )(void*));

CatlExitStatus catl_static_array_destroy(catl_static_array_t* list);

CatlExitStatus catl_static_array_set(catl_static_array_t* list, const size_t idx, const void* data);

CatlExitStatus catl_static_array_get(void** out, const catl_static_array_t* list, const size_t idx);

CatlExitStatus catl_static_array_find_first(size_t* idx_buffer, void** out, const catl_static_array_t* list, const void* value, bool (*compare_func) (const void* fixed_data, const void* iteration_data));

CatlExitStatus catl_static_array_copy(catl_static_array_t** out, const catl_static_array_t* list);

#endif
