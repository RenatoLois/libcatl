#include "catl/hash-algorithms/fnv1a.h"


CatlExitStatus catl_hash_fnv1a_32(uint32_t* out_hash, const unsigned char* data, const size_t length) {
  if (!out_hash || !data) {
    return CATL_STATUS_NULL_POINTER;
  }

  uint32_t hash32 = CATL_HASH_FNV1A_32_OFFSET_BASIS;

  for(size_t i = 0; i < length; i++) {
    hash32 ^= data[i];
    hash32 *= CATL_HASH_FNV1A_32_PRIME;
  }

  *out_hash = hash32;

  return CATL_STATUS_OK;
}

CatlExitStatus catl_hash_fnv1a_64(uint64_t* out_hash, const unsigned char* data, const size_t length) {
  if (!out_hash || !data) {
    return CATL_STATUS_NULL_POINTER;
  }

  uint64_t hash64 = CATL_HASH_FNV1A_64_OFFSET_BASIS;

  for(size_t i = 0; i < length; i++) {
    hash64 ^= data[i];
    hash64 *= CATL_HASH_FNV1A_64_PRIME;
  }

  *out_hash = hash64;

  return CATL_STATUS_OK;
}

