#ifndef CATL_HASHMAP_H
#define CATL_HASHMAP_H

#include "catl/catl_internal/catl_exit_status.h"

typedef struct catl_hashmap_t {
} catl_hashmap_t;

CatlExitStatus catl_linked_list_iterator_create(catl_hashmap_t** out);

CatlExitStatus catl_linked_list_iterator_destroy(catl_hashmap_t* hashmap);

#endif
