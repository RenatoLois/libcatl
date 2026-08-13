#ifndef CATL_ARRAY_LIST_H
#define CATL_ARRAY_LIST_H

#include "catl/catl_internal/catl_exit_status.h"
#include <stddef.h>
#include <stdbool.h>

#define CATL_ARRAY_LIST_INITIAL_CAPACITY 8

typedef struct {
  void** array;
  size_t length;
  size_t capacity;
  size_t data_size;
  void (* delete_function )(void*);
} catl_array_list_t;

CatlExitStatus catl_array_list_create(catl_array_list_t** out, const size_t data_size, const size_t initial_max_lenght, void (* delete_function )(void*));

CatlExitStatus catl_array_list_clear(catl_array_list_t* list);

CatlExitStatus catl_array_list_destroy(catl_array_list_t* list);

CatlExitStatus catl_array_list_push_back(catl_array_list_t* list, const void* data);

CatlExitStatus catl_array_list_push_front(catl_array_list_t* list, const void* data);

CatlExitStatus catl_array_list_pop_back(catl_array_list_t* list);

CatlExitStatus catl_array_list_pop_front(catl_array_list_t* list);

CatlExitStatus catl_array_list_insert(catl_array_list_t* list, const size_t idx, const void* data);

CatlExitStatus catl_array_list_delete(catl_array_list_t* list, const size_t idx);

CatlExitStatus catl_array_list_set(catl_array_list_t* list, const size_t idx, const void* data);

CatlExitStatus catl_array_list_get(void** out, const catl_array_list_t* list, const size_t idx);

CatlExitStatus catl_array_list_find_first(size_t* idx_buffer, void** out, const catl_array_list_t* list, const void* value, bool (*compare_func) (const void* fixed_data, const void* iteration_data));

CatlExitStatus catl_array_list_copy(catl_array_list_t** out, const catl_array_list_t* list);

#endif
