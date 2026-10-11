#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(int) == 4 && sizeof(__SIZE_TYPE__) == 4,
              "original byte and target count representation");
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "original target pointer representation");
extern "C" {
// Complete original memcmp GLOBAL14872, reused by the interpreter's real call.
// Accessible ordinary object-representation storage of count bytes on both sides;
// same/overlapping readable ranges are permitted; no concurrent mutation.
// Zero count performs no memory access. Return is the first unsigned-byte
// difference, or zero when the entire requested range is equal.
int norrath_memory_compare(const void* left, const void* right, __SIZE_TYPE__ count);
}
