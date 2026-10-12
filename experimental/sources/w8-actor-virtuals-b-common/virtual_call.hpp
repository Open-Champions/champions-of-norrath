// Dispatch through a table the way the observed AutoProp methods do: the object's word
// at +72 is a table address, the entry at the given byte offset holds a signed 16-bit
// adjustment at +0 and the function address at +4, and the function is called with
// the object address plus that adjustment as its first argument.
// The host check defines NORRATH_W8AVB_HOST so table and function words can be mapped
// back to host addresses; the EE build uses the words as addresses directly.
#pragma once

namespace w8avb {

typedef unsigned UWord __attribute__((may_alias));
typedef short Half __attribute__((may_alias));

#if defined(NORRATH_W8AVB_HOST)
extern "C" unsigned long long norrath_w8avb_host_base;
extern "C" void* norrath_w8avb_host_function(unsigned word);
inline const unsigned char* address_of(unsigned word) {
    return reinterpret_cast<const unsigned char*>(norrath_w8avb_host_base + word);
}
inline void* function_of(unsigned word) { return norrath_w8avb_host_function(word); }
#else
inline const unsigned char* address_of(unsigned word) {
    return reinterpret_cast<const unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
inline void* function_of(unsigned word) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word));
}
#endif

inline const unsigned char* table_entry(void* self, unsigned byte_offset) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    return address_of(*reinterpret_cast<const UWord*>(object + 72)) + byte_offset;
}

inline void call_slot(void* self, unsigned byte_offset) {
    const unsigned char* entry = table_entry(self, byte_offset);
    typedef void (*Function)(void*);
    Function function = reinterpret_cast<Function>(function_of(*reinterpret_cast<const UWord*>(entry + 4)));
    function(static_cast<unsigned char*>(self) + *reinterpret_cast<const Half*>(entry));
}

inline void call_slot(void* self, unsigned byte_offset, int argument) {
    const unsigned char* entry = table_entry(self, byte_offset);
    typedef void (*Function)(void*, int);
    Function function = reinterpret_cast<Function>(function_of(*reinterpret_cast<const UWord*>(entry + 4)));
    function(static_cast<unsigned char*>(self) + *reinterpret_cast<const Half*>(entry), argument);
}

}  // namespace w8avb
