#include "catl/stack/catl_stack.h"
#include "catl/catl_internal/catl_exit_status.h"

#include <stddef.h>
#include <stdlib.h>

CatlExitStatus catl_stack_create(catl_stack_t** stack, size_t data_size, void (* delete_function )(void*)) {
  catl_stack_t* new_stack = (catl_stack_t*) malloc(sizeof(catl_stack_t));
  if(!new_stack) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  new_stack->list = catl_linked_list_create(data_size, delete_function);
  if(!new_stack->list) {
    free(new_stack);
    return NULL;
  }
  new_stack->length = 0;
  ;
}

CatlExitStatus catl_stack_destroy(catl_stack_t* stack) {
  if(!stack) return CATL_STATUS_NULL_POINTER;
  CatlExitStatus status = catl_linked_list_destroy(stack->list);
  if(status != CATL_STATUS_OK) return status;
  free(stack);
  return CATL_STATUS_OK;
}

bool catl_stack_is_empty(catl_stack_t* stack) {
  return stack->list->head == NULL;
}

CatlExitStatus catl_stack_pop(catl_stack_t* stack) {
  if(!stack) return CATL_STATUS_INVALID_MEMORY;
  if(catl_stack_is_empty(stack)) return CATL_STATUS_INVALID_MEMORY;
  if(stack->list->tail->data) stack->list->delete_function(stack->list->tail->data);
  catl_linked_list_node_t* new_tail = stack->list->tail->previous;
  free(stack->list->tail);
  stack->list->tail = new_tail;
  if(new_tail) new_tail->next = NULL;
  else stack->list->head = NULL;
  stack->list->length--;
  stack->length--;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_stack_push(catl_stack_t* stack, void* value) {
  if(!stack) return CATL_STATUS_INVALID_MEMORY;
  CatlExitStatus status = catl_linked_list_pushback(stack->list, value);
  if(status != CATL_STATUS_OK) return status;
  stack->length++;
  return status;
}

void* catl_stack_top(catl_stack_t* stack) {
  if(!stack || catl_stack_is_empty(stack)) return NULL;
  return stack->list->tail->data;
}


