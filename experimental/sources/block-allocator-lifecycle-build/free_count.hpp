#pragma once
#include "block_alloc.hpp"
extern "C" {
void norrath_block_free(void* object);
int norrath_block_get_number_free();
}
