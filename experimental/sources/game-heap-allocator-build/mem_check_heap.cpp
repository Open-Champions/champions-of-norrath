#include "heap.hpp"
// Complete original144b60/8-byte leaf really returns without checking, writing,
// or calling anything. This is observed source behavior, never a fake service.
extern "C" void norrath_mem_check_heap_real(const char*, int) {}
