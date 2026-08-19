#include "catl/static-array/catl_static_array.h"
#include "catl/catl_internal/catl_exit_status.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

CatlExitStatus catl_static_array_create(
  catl_static_array_t** out,
  size_t data_size,
  size_t capacity
) {
  catl_static_array_t* new_static_array = malloc(sizeof(catl_static_array_t));
  if(!new_static_array) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  if(capacity == 0) capacity = CATL_STATIC_ARRAY_DEFAULT_CAPACITY;

  if(data_size > SIZE_MAX / capacity) return CATL_STATUS_OVERFLOW;

  new_static_array->array = malloc(data_size * capacity);
  if(!new_static_array->array) {
    free(new_static_array);
    return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  }

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
  if(!list) return CATL_STATUS_NULL_POINTER;

  if(list->capacity <= idx) {
    return CATL_STATUS_OUT_OF_RANGE;
  }

  memcpy(
    (char*) (list->array) + (idx * list->data_size),
    data,
    list->data_size
  );

  return CATL_STATUS_OK;
}

CatlExitStatus catl_static_array_get(void** out, const catl_static_array_t* list, const size_t idx) {
  if(!list || !out) return CATL_STATUS_NULL_POINTER;

  if(list->capacity <= idx) {
    return CATL_STATUS_OUT_OF_RANGE;
  }

  *out = (char *) (list->array) + (idx * list->data_size);

  return CATL_STATUS_OK;
}

CatlExitStatus catl_static_array_find_first(
  size_t* idx_buffer,
  void** out,
  const catl_static_array_t* list,
  const void* value,
  bool (*compare_func) (const void* fixed_data, const void* iteration_data)
) {
  if(!list || !compare_func) return CATL_STATUS_NULL_POINTER;
  
  for(size_t i=0; i < list->capacity; i++) {
    void* current_ptr = (char*)(list->array) + (i * list->data_size);

    if( compare_func( value, current_ptr ) ) {
      if(idx_buffer) *idx_buffer = i;
      if(out) *out = current_ptr;
      return CATL_STATUS_OK;
    }
  }

  return CATL_STATUS_CANNOT_FIND;
}

CatlExitStatus catl_static_array_copy(catl_static_array_t** out, const catl_static_array_t* list) {
  if(!list || !out) return CATL_STATUS_NULL_POINTER;

  catl_static_array_t* new_array;
  CatlExitStatus creation_status = catl_static_array_create(&new_array, list->data_size, list->capacity);

  if(creation_status != CATL_STATUS_OK) {
    return creation_status;
  }

  memcpy(new_array->array, list->array, (list->data_size * list->capacity));

  *out = new_array;
  return CATL_STATUS_OK;
}
