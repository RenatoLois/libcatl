#include "catl/linked-list/catl_linked_list_iterator.h"
#include "catl/catl_internal/catl_exit_status.h"
#include <stdlib.h>



CatlExitStatus catl_linked_list_iterator_create(catl_linked_list_iterator_t** out, const catl_linked_list_t* list) {
  if (!list) return CATL_STATUS_NULL_POINTER;
  
  catl_linked_list_iterator_t* iterator = malloc(sizeof(catl_linked_list_iterator_t));
  if (!iterator) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  
  iterator->list = list;
  iterator->current = list->head;
  
  *out = iterator;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_destroy(catl_linked_list_iterator_t* iterator) {
  if (!iterator) return CATL_STATUS_NULL_POINTER;
  free(iterator);
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_next(catl_linked_list_iterator_t* iterator) {
  if(!iterator) return CATL_STATUS_NULL_POINTER;
  if(!iterator->current) return CATL_STATUS_INVALID_STATE;

  if(iterator->current->next) {
    iterator->current = iterator->current->next;
  } else {
    return CATL_STATUS_LIMIT_EXCEEDED;
  }

  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_previous(catl_linked_list_iterator_t* iterator) {
  if(!iterator) return CATL_STATUS_NULL_POINTER;
  if(!iterator->current) return CATL_STATUS_INVALID_STATE;

  if(iterator->current->previous) {
    iterator->current = iterator->current->previous;
  } else {
    return CATL_STATUS_LIMIT_EXCEEDED;
  }

  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_has_next(bool* out, const catl_linked_list_iterator_t* iterator) {
  if(!iterator || !out) return CATL_STATUS_NULL_POINTER;

  if(!iterator->current || !iterator->current->next) {
    *out = false;
  } else {
    *out = true;
  }

  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_has_previous(bool* out, const catl_linked_list_iterator_t* iterator) {
  if(!iterator || !out) return CATL_STATUS_NULL_POINTER;

  if(!iterator->current || !iterator->current->previous) {
    *out = false;
  } else {
    *out = true;
  }

  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_reset_to_head(catl_linked_list_iterator_t* iterator) {
  if (!iterator) return CATL_STATUS_NULL_POINTER;
  iterator->current = iterator->list->head;
  return CATL_STATUS_OK;
}

CatlExitStatus catl_linked_list_iterator_reset_to_tail(catl_linked_list_iterator_t* iterator) {
  if (!iterator) return CATL_STATUS_NULL_POINTER;
  iterator->current = iterator->list->tail;
  return CATL_STATUS_OK;
}
