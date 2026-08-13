#include "catl/static-array/catl_static_array.h"
#include "catl/catl_internal/catl_exit_status.h"
#include "stdlib.h"


CatlExitStatus catl_array_create(catl_static_array_t** out, size_t data_size, void (* delete_function )(void*), size_t capacity) {
  catl_static_array_t* new_static_array = malloc(sizeof(catl_static_array_t));
  if(!new_static_array) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  if(capacity == 0) capacity = CATL_STATIC_ARRAY_DEFAULT_CAPACITY;

  new_static_array->array = malloc(data_size * capacity);
  if(!new_static_array->array) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  new_static_array->capacity = capacity;
  new_static_array->data_size = data_size;

  *out = new_static_array;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_static_array_destroy(catl_static_array_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  free(list->array);
  free(list);

  return CATL_STATUS_OK;
}

CatlExitStatus catl_static_array_set(catl_static_array_t* list, const size_t idx, const void* data) {
  
}

CatlExitStatus catl_static_array_get(void** out, const catl_static_array_t* list, const size_t idx)

CatlExitStatus catl_static_array_find_first(size_t* idx_buffer, void** out, catl_static_array_t* list, void* value, bool (*compare_func) (void* fixed_data, void* iteration_data))

CatlExitStatus catl_static_array_copy(catl_static_array_t** out, catl_static_array_t* list)

CatlExitStatus catl_static_array_get(void** out, catl_static_array_t* list, size_t idx)

CatlExitStatus catl_static_array_find_first(size_t* idx_buffer, void** out, catl_static_array_t* list, void* value, bool (*compare_func) (void* fixed_data, void* iteration_data))

CatlExitStatus catl_static_array_copy(catl_static_array_t** out, catl_static_array_t* list)

#
