#include "catl/pair/catl_pair.h"
#include <string.h>

catl_pair_t* catl_pair_create(
  size_t size_first, size_t size_second,
  void (* first_delete_function)(void*),
  void (* second_delete_function)(void*)
) {
  catl_pair_t* pair = (catl_pair_t*) malloc(sizeof(catl_pair_t));
  if(!pair) return NULL;
  pair->size_first = size_first;
  pair->size_second = size_second;
  pair->first = NULL;
  pair->second = NULL;
  pair->first_delete_function = first_delete_function ? first_delete_function : free;
  pair->second_delete_function = second_delete_function ? second_delete_function : free;
  return pair;
}

CatlExitStatus catl_pair_destroy(catl_pair_t *pair) {
  if(!pair) return CATL_STATUS_INVALID_MEMORY;
  if(pair->first) pair->first_delete_function(pair->first);
  if(pair->second) pair->second_delete_function(pair->second);
  free(pair);
  return CATL_STATUS_OK;
}

CatlExitStatus catl_pair_set_first(catl_pair_t* pair, void* data) {
  if(!pair || !data) return CATL_STATUS_INVALID_MEMORY;
  if(pair->first) pair->first_delete_function(pair->first);
  pair->first = malloc(pair->size_first);
  if(!pair->first) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  memcpy(pair->first, data, pair->size_first);
  return CATL_STATUS_OK;
}

CatlExitStatus catl_pair_set_second(catl_pair_t* pair, void* data) {
  if(!pair || !data) return CATL_STATUS_INVALID_MEMORY;
  if(pair->second) pair->second_delete_function(pair->second);
  pair->second = malloc(pair->size_second);
  if(!pair->second) return CATL_STATUS_FAILED_TO_ALLOCATE_MEMORY;
  memcpy(pair->second, data, pair->size_second);
  return CATL_STATUS_OK;
}

