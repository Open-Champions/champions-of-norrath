#pragma once
#include "block_alloc.hpp"
extern "C" {
// Actual unsized dataBlock symbol at0x519440. Required mapped writable view
// is947968 bytes; this declaration does not invent a sized owner.
extern unsigned char norrath_block_data_pool_view[];
void norrath_block_alloc_init();
}
