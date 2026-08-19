#include "catl/array-list/catl_array_list.h"
#include "catl/catl_internal/catl_exit_status.h"
#include "catl/static-array/catl_static_array.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static CatlExitStatus catl_array_list_duplicate_capacity(catl_static_array_t* static_array) {
  if(static_array->capacity > SIZE_MAX / 2) {
    return CATL_STATUS_OVERFLOW;
  }

  size_t new_capacity = static_array->capacity * 2;

  if(new_capacity > 0 && static_array->data_size > SIZE_MAX / new_capacity) {
    return CATL_STATUS_OVERFLOW;
  }

  void* new_array = realloc(static_array->array, new_capacity * static_array->data_size);

  if(!new_array) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  static_array->array = new_array;
  static_array->capacity = new_capacity;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_create(catl_array_list_t** out, const size_t data_size,const size_t initial_capacity) {
  if(!out) return CATL_STATUS_NULL_POINTER;
  
  catl_array_list_t* new_array;
  new_array = (catl_array_list_t*) malloc(sizeof(catl_array_list_t));
  if(!new_array) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  
  if (initial_capacity == 0) {
    new_array->capacity = CATL_ARRAY_LIST_INITIAL_CAPACITY;
  } else {
    new_array->capacity = initial_capacity;
  }

  new_array->length = 0;
  new_array->data_size = data_size;

  CatlExitStatus static_array_creation_status = catl_static_array_create(&new_array->static_array, new_array->data_size, new_array->capacity);

  if(static_array_creation_status != CATL_STATUS_OK) {
    free(new_array);
    return static_array_creation_status;
  }

  *out = new_array;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_destroy(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  
  CatlExitStatus destroy_status = catl_static_array_destroy(list->static_array);
  free(list);

  return destroy_status; // CATL_STATUS_OK or ERROR
}

CatlExitStatus catl_array_list_push_back(catl_array_list_t* list, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;

  if(list->length >= list->capacity) {
    CatlExitStatus status = catl_array_list_duplicate_capacity(list->static_array);
    if(status != CATL_STATUS_OK) return status;
    list->capacity = list->static_array->capacity;
  }

  memcpy(
    (char *) (list->static_array->array) + (list->data_size * list->length),
    data,
    list->data_size
  );

  list->length++;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_push_front(catl_array_list_t* list, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(list->length >= list->capacity) {
    CatlExitStatus status = catl_array_list_duplicate_capacity(list->static_array);
    if(status != CATL_STATUS_OK) return status;
    list->capacity = list->static_array->capacity;
  }

  char* begin_ptr = (char *) (list->static_array->array);

  memmove((char *) begin_ptr + list->data_size, begin_ptr, list->length * list->data_size);

  memcpy(
    begin_ptr,
    data,
    list->data_size
  );

  list->length++;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_pop_back(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(list->length == 0) return CATL_STATUS_EMPTY_LIST;
  list->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_pop_front(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(list->length == 0) return CATL_STATUS_EMPTY_LIST;

  char* begin_ptr = (char *) (list->static_array->array);

  memmove(begin_ptr, begin_ptr + list->data_size, (list->length - 1) * list->data_size);
  list->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_insert(catl_array_list_t* list, const size_t idx, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(idx > list->length) return CATL_STATUS_OUT_OF_RANGE;
  if(list->length >= list->capacity) {
    CatlExitStatus status = catl_array_list_duplicate_capacity(list->static_array);
    if(status != CATL_STATUS_OK) return status;
    list->capacity = list->static_array->capacity;
  }

  char *idx_ptr = (char *) (list->static_array->array) + list->data_size * idx;

  memmove(
    idx_ptr + list->data_size,
    idx_ptr,
    (list->length - idx) * list->data_size
  );

  memcpy(
    idx_ptr,
    data,
    list->data_size
  );

  list->length++;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_set(catl_array_list_t* list, const size_t idx, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;

  memcpy(
    (char *) (list->static_array->array) + list->data_size * idx,
    data,
    list->data_size
  );

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_get(void** out, const catl_array_list_t* list, const size_t idx) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;

  *out = (void*) ((char *) (list->static_array->array) + list->data_size * idx);

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_find_first(
  size_t* idx_buffer,
  void** out,
  const catl_array_list_t* list,
  const void* value, bool (*compare_func) (const void* fixed_data, const void* iteration_data)
) {
  if(!list || !compare_func) return CATL_STATUS_NULL_POINTER;
  
  char *current;

  for(size_t i=0; i < list->length; i++) {
    current = (char *) (list->static_array->array) + list->data_size * i;

    if(compare_func(value, current)) {
      if(idx_buffer) *idx_buffer = i;
      if(out) *out = current;
      return CATL_STATUS_OK;
    }
  }

  return CATL_STATUS_CANNOT_FIND;
}

CatlExitStatus catl_array_list_copy(catl_array_list_t** out, const catl_array_list_t* list) {  // verify this later
  catl_array_list_t* new_array_list;
  CatlExitStatus creation_status = catl_array_list_create(
    &new_array_list,
    list->data_size,
    list->capacity
  );

  if(creation_status != CATL_STATUS_OK) return creation_status;

  memcpy(new_array_list->static_array->array, list->static_array->array, list->data_size * list->length);
  new_array_list->length = list->length;

  *out = new_array_list;

  return CATL_STATUS_OK;
}
