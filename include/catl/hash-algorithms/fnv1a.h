#ifndef CATL_HASH_FNV1A_H
#define CATL_HASH_FNV1A_H

#include <stddef.h>
#include <stdint.h>
#include "catl/catl_internal/catl_exit_status.h"


#define CATL_HASH_FNV1A_32_OFFSET_BASIS 0x811c9dc5
#define CATL_HASH_FNV1A_32_PRIME 0x01000193

#define CATL_HASH_FNV1A_64_OFFSET_BASIS 0xcbf29ce484222325
#define CATL_HASH_FNV1A_64_PRIME 0x00000100000001b3


CatlExitStatus catl_hash_fnv1a_32(uint32_t* out_hash, const unsigned char* data, const size_t length);

CatlExitStatus catl_hash_fnv1a_64(uint64_t* out_hash, const unsigned char* data, const size_t length);

#endif
