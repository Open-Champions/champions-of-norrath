#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4,
              "observed 32-bit word representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target address bridge");
extern "C" {
// A minimum four-byte view, not a recovered allocation or owned definition.
extern unsigned char norrath_constructor_list_pool_view[];
// Returns the original node+12 word. Original C++ return type is unresolved.
unsigned norrath_list_remove(unsigned char* node);
}
