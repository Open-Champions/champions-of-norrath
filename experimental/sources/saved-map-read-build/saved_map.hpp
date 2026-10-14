#pragma once
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(void*) == 4 &&
              sizeof(__UINTPTR_TYPE__) == 4, "target32 byte address representation");
extern "C" {
// Original maptiles is a sized93312-byte object. No payload is emitted here.
extern unsigned char norrath_saved_map_tiles_bytes[];
void norrath_map_read_saved_bytes(unsigned char* source);
}
namespace norrath_saved_map_detail {
inline unsigned address(const void* p) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(p)); }
inline unsigned char* bytes(unsigned a) { return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(a)); }
}
