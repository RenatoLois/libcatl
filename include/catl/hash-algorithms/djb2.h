#ifndef CATL_HASH_DJB2_H
#define CATL_HASH_DJB2_H

#include "catl/catl_internal/catl_exit_status.h"
#include <stddef.h>


CatlExitStatus catl_hash_djb2(size_t* out, const char* data, const size_t length);

#endif

