#pragma once
#include "number_parser.hpp"
extern "C" {
unsigned char* norrath_player_calloc_reentrant(unsigned char*,unsigned,unsigned);
unsigned char* norrath_player_malloc_reentrant(unsigned char*,unsigned);
void* norrath_block_memory_set(void*,int,unsigned);
}
