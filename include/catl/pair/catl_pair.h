#ifndef CATL_PAIR_H
#define CATL_PAIR_H

#include "catl/catl_internal/catl_exit_status.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct {
  void* first;
  void* second;
  size_t size_first;
  size_t size_second;
  void (* first_delete_function)(void*);
  void (* second_delete_function)(void*);
} catl_pair_t;

catl_pair_t* catl_pair_create(
  size_t size_first,
  size_t size_second,
  void (* first_delete_function)(void*),
  void (* second_delete_function)(void*)
);

CatlExitStatus catl_pair_destroy(catl_pair_t* pair);

static inline void* catl_pair_get_first(catl_pair_t* pair);


static inline void* catl_pair_get_second(catl_pair_t* pair);

CatlExitStatus catl_pair_set_first(catl_pair_t* pair, void* data);

CatlExitStatus catl_pair_set_second(catl_pair_t* pair, void* data);


// inline functiones implementation
static inline CatlExitStatus catl_pair_get_first(void** buffer, catl_pair_t* pair) { 
  (*buffer) = (pair && pair->first) ? pair->first : NULL;
  return CATL_STATUS_OK;
}

static inline void* catl_pair_get_second(catl_pair_t* pair) {
  return (pair && pair->second) ? pair->second : NULL;
}

#endif
