#ifndef CATL_LINKED_LIST_ITERATOR_H
#define CATL_LINKED_LIST_ITERATOR_H

#include "catl/catl_internal/catl_exit_status.h"
#include "catl/linked-list/catl_linked_list.h"
#include <stddef.h>

typedef struct catl_linked_list_iterator_t {
  const catl_linked_list_t* list;
  catl_linked_list_node_t* current;
} catl_linked_list_iterator_t;

CatlExitStatus catl_linked_list_iterator_create(catl_linked_list_iterator_t** out, const catl_linked_list_t* list);

CatlExitStatus catl_linked_list_iterator_destroy(catl_linked_list_iterator_t* iterator);

CatlExitStatus catl_linked_list_iterator_next(catl_linked_list_iterator_t* iterator);

CatlExitStatus catl_linked_list_iterator_previous(catl_linked_list_iterator_t* iterator);

CatlExitStatus catl_linked_list_iterator_has_next(bool* out, const catl_linked_list_iterator_t* iterator);

CatlExitStatus catl_linked_list_iterator_has_previous(bool* out, const catl_linked_list_iterator_t* iterator);

CatlExitStatus catl_linked_list_iterator_reset_to_head(catl_linked_list_iterator_t* iterator);

CatlExitStatus catl_linked_list_iterator_reset_to_tail(catl_linked_list_iterator_t* iterator);

#endif
