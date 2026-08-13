#ifndef CATL_QUEUE_H
#define CATL_QUEUE_H

#include "catl/catl_internal/catl_exit_status.h"
#include "catl/linked-list/catl_linked_list.h"
#include <stdbool.h>

typedef struct {
  catl_linked_list_t* list;
  size_t length;
  void (* delete_function )(void*);
} catl_queue_t;

CatlExitStatus catl_queue_create(catl_queue_t** queue, size_t size, void (* delete_function )(void*));

CatlExitStatus catl_queue_destroy(catl_queue_t* queue);

CatlExitStatus catl_queue_is_empty(bool* value, catl_queue_t* queue);

CatlExitStatus catl_queue_push(catl_queue_t* queue, void* value);

CatlExitStatus catl_queue_pop(catl_queue_t* queue);

CatlExitStatus catl_stack_front(void** out, catl_queue_t* queue);

#endif


