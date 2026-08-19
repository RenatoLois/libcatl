#include "catl/hash-algorithms/djb2.h"


CatlExitStatus catl_hash_djb2(size_t* out, const char* data, const size_t length) {
  if(!data || !out) return CATL_STATUS_NULL_POINTER;

  size_t hash = 5381;
  unsigned char *current = (unsigned char*) data;
  if(length == 0) {
    while(*current) {
      hash = 33 * hash + *current;
      current++;
    }
  } else {
    for(size_t i=0; i<length; i++) {
      hash = (hash << 5) + hash + *current; // hash * 33 + c
      current++;
    }
  }

  *out = hash;
  return CATL_STATUS_OK;
}
