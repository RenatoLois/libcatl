#ifndef CATL_LINKED_LIST_H
#define CATL_LINKED_LIST_H

#include "catl/catl_internal/catl_exit_status.h"
#include <stddef.h>
#include <stdbool.h>

typedef struct catl_linked_list_node_t{
  void* data;
  struct catl_linked_list_node_t* previous;
  struct catl_linked_list_node_t* next;
} catl_linked_list_node_t;

typedef struct {
  catl_linked_list_node_t* head;
  catl_linked_list_node_t* tail;
  size_t length;
  size_t data_size;
  void (* delete_function )(void*);
} catl_linked_list_t;

CatlExitStatus catl_linked_list_create(catl_linked_list_t** out, const size_t data_size, void (* delete_function )(void*));

CatlExitStatus catl_linked_list_clear(catl_linked_list_t* list);

CatlExitStatus catl_linked_list_destroy(catl_linked_list_t* list);

CatlExitStatus catl_linked_list_push_back(catl_linked_list_t* list, const void* data);

CatlExitStatus catl_linked_list_push_front(catl_linked_list_t* list, const void* data);

CatlExitStatus catl_linked_list_pop_back(catl_linked_list_t* list);

CatlExitStatus catl_linked_list_pop_front(catl_linked_list_t* list);

CatlExitStatus catl_linked_list_insert(catl_linked_list_t* list, const size_t idx, const void* data);

CatlExitStatus catl_linked_list_delete(catl_linked_list_t* list, const size_t idx);

CatlExitStatus catl_linked_list_set(catl_linked_list_t* list, const size_t idx, const void* data);

CatlExitStatus catl_linked_list_get(void** out, const catl_linked_list_t* list, const size_t idx);

CatlExitStatus catl_linked_list_find_first(
  size_t* idx_buffer,
  void** out,
  const catl_linked_list_t* list,
  const void* value,
  bool (*compare_func) (const void* fixed_data, const void* iteration_data)
);

CatlExitStatus catl_linked_list_copy(void** out, const catl_linked_list_t* list);

#endif
