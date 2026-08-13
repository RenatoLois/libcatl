#include "catl/array-list/catl_array_list.h"
#include "catl/catl_internal/catl_exit_status.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Duplicate list capacity by 2; return CATL_STATUS_OVERFLOW, CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY, or CATL_STATUS_OK
static CatlExitStatus catl_array_list_duplicate_capacity(catl_array_list_t* list) {
  if(list->capacity > SIZE_MAX / 2) {
    return CATL_STATUS_OVERFLOW;
  }
  size_t new_capacity = list->capacity * 2;
  void** new_array = realloc(list->array, new_capacity * sizeof(void*));
  if(!new_array) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  list->array = new_array;
  list->capacity = new_capacity;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_create(catl_array_list_t** out, const size_t data_size,const size_t initial_capacity, void (* delete_function )(void*)) {
  if(!out) return CATL_STATUS_NULL_POINTER;

  *out = (catl_array_list_t*) malloc(sizeof(catl_array_list_t));
  if(!(*out)) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  
  if (initial_capacity == 0) {
    (*out)->capacity = CATL_ARRAY_LIST_INITIAL_CAPACITY;
  } else {
    (*out)->capacity = initial_capacity;
  }

  (*out)->length = 0;
  (*out)->data_size = data_size;
  (*out)->delete_function = delete_function ? delete_function : free;

  (*out)->array = (void**) malloc(sizeof(void*) * (*out)->capacity);
  if(!(*out)->array) {
    free((*out));
    return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  }

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_clear(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  for(size_t i=0; i < list->length; i++) {
    if(list->array[i]) list->delete_function(list->array[i]);
  }
  list->length = 0;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_destroy(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  
  CatlExitStatus clear_status = catl_array_list_clear(list);

  if(clear_status != CATL_STATUS_OK) {
    return clear_status;
  }

  free(list->array);
  free(list);
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_push_back(catl_array_list_t* list, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;

  if(list->length >= list->capacity) {
    CatlExitStatus status = catl_array_list_duplicate_capacity(list);
    if(status != CATL_STATUS_OK) return status;
  }

  void* new_data = malloc(list->data_size);
  if(!new_data) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  memcpy(new_data, data, list->data_size);

  list->array[list->length] = new_data;
  list->length++;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_push_front(catl_array_list_t* list, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(list->length >= list->capacity) {
    CatlExitStatus status = catl_array_list_duplicate_capacity(list);
    if(status != CATL_STATUS_OK) return status;
  }
  void* new_data = malloc(list->data_size);
  if(!new_data) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  memcpy(new_data, data, list->data_size);
  memmove(list->array + 1, list->array, list->length * sizeof(void*));
  list->array[0] = new_data;
  list->length++;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_pop_back(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(list->length == 0) return CATL_STATUS_EMPTY_LIST;
  if(list->array[list->length - 1]) list->delete_function(list->array[list->length - 1]);
  list->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_pop_front(catl_array_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(list->length == 0) return CATL_STATUS_EMPTY_LIST;
  if(list->array[0]) list->delete_function(list->array[0]);
  memmove(list->array, list->array+ 1, (list->length - 1) * sizeof(void*));
  list->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_insert(catl_array_list_t* list, const size_t idx, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(idx > list->length) return CATL_STATUS_OUT_OF_RANGE;
  if(list->length >= list->capacity) {
    CatlExitStatus status = catl_array_list_duplicate_capacity(list);
    if(status != CATL_STATUS_OK) return status;
  }
  void* new_data = malloc(list->data_size);
  if(!new_data) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  memcpy(new_data, data, list->data_size);
  
  memmove(list->array + 1 + idx, list->array + idx, (list->length - idx) * sizeof(void*));
  list->array[idx] = new_data;
  list->length++;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_delete(catl_array_list_t* list, const size_t idx) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;
  if(list->array[idx]) list->delete_function(list->array[idx]);
  memmove(list->array+idx, list->array+idx + 1, (list->length - idx - 1) * sizeof(void*));
  list->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_set(catl_array_list_t* list, const size_t idx, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;
  void* new_data = malloc(list->data_size);
  if(!new_data) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  memcpy(new_data, data, list->data_size);
  if(list->array[idx]) list->delete_function(list->array[idx]);
  list->array[idx] = new_data;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_get(void** out, const catl_array_list_t* list, const size_t idx) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;
  (*out) = list->array[idx];
  return CATL_STATUS_OK;
}

CatlExitStatus catl_array_list_find_first(
  size_t* idx_buffer,
  void** out,
  const catl_array_list_t* list,
  const void* value, bool (*compare_func) (const void* fixed_data, const void* iteration_data)
) {
  if(!list || !compare_func) return CATL_STATUS_NULL_POINTER;
  
  for(size_t i=0; i < list->length; i++) {
    if(compare_func(value, list->array[i])) {
      if(idx_buffer) *idx_buffer = i;
      if(out) *out = list->array[i];
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
    list->capacity,
    list->delete_function
  );

  if(creation_status != CATL_STATUS_OK) return creation_status;

  for(size_t i = 0; i < list->length; i++) {
    new_array_list->array[i] = malloc(new_array_list->data_size);
    if( !new_array_list->array[i] ) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
    memcpy(new_array_list->array[i], list->array[i], new_array_list->data_size);
  }

  return CATL_STATUS_OK;
}
