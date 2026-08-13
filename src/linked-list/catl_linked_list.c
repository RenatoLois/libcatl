#include "catl/linked-list/catl_linked_list.h"

#include "catl/catl_internal/catl_exit_status.h"
#include "stdlib.h"
#include "string.h"
#include <stddef.h>

static catl_linked_list_node_t* catl_linked_list_node_create(const void* data, const size_t data_size) {
  catl_linked_list_node_t* node = (catl_linked_list_node_t*) malloc(sizeof(catl_linked_list_node_t));
  if(!node) return NULL;
  node->data = malloc(data_size);
  if(!node->data) {
    free(node);
    return NULL;
  }
  memcpy(node->data, data, data_size);
  node->next = node->previous = NULL;
  return node;
}

static catl_linked_list_node_t* catl_linked_list_node_get_at_idx(const size_t idx, const catl_linked_list_t* list) {
  catl_linked_list_node_t* current_node;
  size_t steps;

  if(idx <= list->length / 2) {
    current_node = list->head;
    steps = idx;
    while(steps--) {
      current_node = current_node->next;
    }
  } else {
    current_node = list->tail;
    steps = list->length - idx - 1;
    while(steps--) {
      current_node = current_node->previous;
    }
  }

  return current_node;
}

CatlExitStatus catl_linked_list_create(catl_linked_list_t** out, const size_t size, void (* delete_function )(void*)) {
  if (!out) return CATL_STATUS_NULL_POINTER;
  if (*out) return CATL_STATUS_ALREADY_INITIALIZED;

  if(size == 0) return CATL_STATUS_INVALID_VALUE;

  (*out) = (catl_linked_list_t*) malloc(sizeof(catl_linked_list_t));
  if(!(*out)) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  (*out)->head = (*out)->tail = NULL;
  (*out)->length = 0;
  (*out)->delete_function = delete_function ? delete_function : free;
  (*out)->data_size = size;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_clear(catl_linked_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  catl_linked_list_node_t* current_node = list->head;
  while(current_node) {     
    catl_linked_list_node_t* next_node = current_node->next;
    if(current_node->data) list->delete_function(current_node->data);
    free(current_node);
    current_node = next_node;
  }
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_destroy(catl_linked_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;

  CatlExitStatus clear_status = catl_linked_list_clear(list);

  if(clear_status != CATL_STATUS_OK) {
    return clear_status;
  }

  free(list);
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_push_back(catl_linked_list_t* list, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  catl_linked_list_node_t* node = catl_linked_list_node_create(data, list->data_size);
  if(!node) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  node->next = NULL;
  if(list->tail == NULL) {
    node->previous = NULL;
    list->head = list->tail = node;
  } else {
    node->previous = list->tail;
    list->tail->next = node;
    list->tail = node;
  }
  list->length++;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_push_front(catl_linked_list_t* list, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;

  catl_linked_list_node_t* node = catl_linked_list_node_create(data, list->data_size);

  if(!node) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  node->previous = NULL;

  if(list->head == NULL) {
    node->next = NULL;
    list->head = list->tail = node;
  } else {
    node->next = list->head;
    list->head->previous = node;
    list->head = node;
  }
  list->length++;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_pop_back(catl_linked_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(list->length == 0) return CATL_STATUS_EMPTY_LIST;
  return catl_linked_list_delete(list, list->length-1);
}

CatlExitStatus catl_linked_list_pop_front(catl_linked_list_t* list) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(list->length == 0) return CATL_STATUS_EMPTY_LIST;
  return catl_linked_list_delete(list, 0);
}

CatlExitStatus catl_linked_list_insert(catl_linked_list_t* list, const size_t idx, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(idx == 0) {
    return catl_linked_list_push_front(list, data);
  } else if(idx == list->length) {
    return catl_linked_list_push_back(list, data);
  } else if(idx > list->length) {
    return CATL_STATUS_OUT_OF_RANGE;
  }

  catl_linked_list_node_t* node = catl_linked_list_node_create(data, list->data_size);
  if(!node) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;

  catl_linked_list_node_t* current_node = catl_linked_list_node_get_at_idx(idx, list);

  current_node->next->previous = node;
  node->next = current_node->next;
  node->previous = current_node;
  current_node->next = node;
  list->length++;
  
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_delete(catl_linked_list_t* list, const size_t idx) {
  if(!list) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;

  catl_linked_list_node_t* current_node = catl_linked_list_node_get_at_idx(idx, list);

  if(current_node->data) list->delete_function(current_node->data);
  if(current_node != list->tail) {
    current_node->next->previous = current_node->previous;
  } else {
    list->tail = current_node->previous;
  }
  
  if(current_node != list->head) {
    current_node->previous->next = current_node->next;
  } else {
    list->head = current_node->next;
  }
  free(current_node);
  list->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_set(catl_linked_list_t* list, const size_t idx, const void* data) {
  if(!list || !data) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) return CATL_STATUS_OUT_OF_RANGE;
  
  void* new_data = malloc(list->data_size);
  if(!new_data) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  
  memcpy(new_data, data, list->data_size);
  
  catl_linked_list_node_t* current_node = catl_linked_list_node_get_at_idx(idx, list);

  if(current_node->data) list->delete_function(current_node->data);
  current_node->data = new_data;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_get(void** out, const catl_linked_list_t* list, const size_t idx) {
  if(!list || !out) return CATL_STATUS_NULL_POINTER;
  if(idx >= list->length) {
    (*out) = NULL;
    return CATL_STATUS_OUT_OF_RANGE;
  }

  catl_linked_list_node_t* current_node = catl_linked_list_node_get_at_idx(idx, list);

  (*out) = current_node->data;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_find_first(
  size_t* idx_buffer,
  void** out,
  const catl_linked_list_t* list,
  const void* value, bool (*compare_func) (const void* data_1, const void* data_2)
) {
  if(!list || !compare_func) return CATL_STATUS_NULL_POINTER;

  catl_linked_list_node_t* current_node = list->head;

  size_t idx = 0;

  while(current_node && !compare_func(value, current_node->data)) {
    idx++;
    current_node = current_node->next;
  }

  if(idx == list->length) return CATL_STATUS_CANNOT_FIND;

  if(idx_buffer) {
    *idx_buffer = idx;
  }

  if(out) {
    *out = current_node->data;
  }

  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_copy(void** out, const catl_linked_list_t* list) {
  if(!list || !out) return CATL_STATUS_NULL_POINTER;

  catl_linked_list_t* new_list;
  CatlExitStatus creation_status = catl_linked_list_create(&new_list, list->data_size, list->delete_function);
  if(creation_status != CATL_STATUS_OK) {
    return creation_status;
  }

  catl_linked_list_node_t* current_node = list->head;
  while(current_node) {
    CatlExitStatus push_back_status = catl_linked_list_push_back(new_list, current_node->data);
    if(push_back_status != CATL_STATUS_OK) {
      catl_linked_list_destroy(new_list);
      return push_back_status;
    }
    current_node = current_node->next;
  }

  *out = new_list;
  return CATL_STATUS_OK;
}
