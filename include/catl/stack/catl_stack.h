#ifndef CATL_STACK_H
#define CATL_STACK_H

#include "catl/catl_internal/catl_exit_status.h"
#include "catl/linked-list/catl_linked_list.h"
#include <stdbool.h>

typedef struct {
  catl_linked_list_t* list;
  size_t length;
  void (* delete_function )(void*);
} catl_stack_t;

CatlExitStatus catl_stack_create(catl_stack_t** stack, size_t size, void (* delete_function )(void*));

CatlExitStatus catl_stack_destroy(catl_stack_t* stack);

CatlExitStatus catl_stack_is_empty(bool* value, catl_stack_t* stack);

CatlExitStatus catl_stack_push(catl_stack_t* stack, void* value);

CatlExitStatus catl_stack_pop(catl_stack_t* stack);

CatlExitStatus catl_stack_top(void** out, catl_stack_t* stack);

#endif

