#pragma once
// Little-endian field access for the animation, object and creature sources of package w9-animation-object-c. Records are
// addressed as byte offsets because no original class declaration is claimed.
// A host test defines NORRATH_HOST_ARENA and supplies the two hooks below, since a
// 64-bit host cannot hold a 32-bit target pointer in a record word.
static_assert(__CHAR_BIT__ == 8 && sizeof(unsigned) == 4 && sizeof(float) == 4,
              "target scalar representation");
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, "target byte order");
#ifndef NORRATH_HOST_ARENA
static_assert(sizeof(void*) == 4 && sizeof(__UINTPTR_TYPE__) == 4,
              "target32 pointer words");
#endif
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" unsigned norrath_host_address(const unsigned char* pointer);
// Stand-in for calling a target function word whose arguments are (pointer, int).
extern "C" void norrath_host_call_pointer_int(unsigned function, unsigned char* first, int second);
// Stand-in for a target function word taking (pointer, pointer, int, float).
extern "C" void norrath_host_call_pointer_pointer_int_float(unsigned function, unsigned char* first,
                                                           unsigned char* second, int third, float fourth);
#endif
namespace norrath_aoc {
typedef unsigned __attribute__((may_alias)) word_t;
typedef unsigned short __attribute__((may_alias)) half_t;
typedef float __attribute__((may_alias)) float_t_;
// Records are naturally aligned in the target, so accesses are single loads and stores.
inline unsigned load_word(const unsigned char* p) { return *reinterpret_cast<const word_t*>(p); }
inline void store_word(unsigned char* p, unsigned value) { *reinterpret_cast<word_t*>(p) = value; }
inline unsigned load_half(const unsigned char* p) { return *reinterpret_cast<const half_t*>(p); }
inline void store_half(unsigned char* p, unsigned value) { *reinterpret_cast<half_t*>(p) = static_cast<unsigned short>(value); }
inline int load_signed_half(const unsigned char* p) { return static_cast<short>(load_half(p)); }
inline int load_signed_byte(const unsigned char* p) { return static_cast<signed char>(*p); }
inline float load_float(const unsigned char* p) { return *reinterpret_cast<const float_t_*>(p); }
inline void store_float(unsigned char* p, float value) { *reinterpret_cast<float_t_*>(p) = value; }
inline unsigned char* load_pointer(const unsigned char* p) {
    const unsigned word = load_word(p);
#ifdef NORRATH_HOST_ARENA
    return word == 0 ? nullptr : norrath_host_pointer(word);
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline void store_pointer(unsigned char* p, const unsigned char* value) {
#ifdef NORRATH_HOST_ARENA
    store_word(p, value == nullptr ? 0u : norrath_host_address(value));
#else
    store_word(p, static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(value)));
#endif
}
// Calls the virtual slot at byte `slot` of the vtable word at self+72. A slot is eight
// bytes: a signed halfword that adjusts `this`, then the function word at +4.
inline void call_virtual_pointer_int(unsigned char* self, unsigned slot, int argument) {
    const unsigned char* entry = load_pointer(self + 72) + slot;
    const int adjust = load_signed_half(entry);
    const unsigned function = load_word(entry + 4);
#ifdef NORRATH_HOST_ARENA
    norrath_host_call_pointer_int(function, self + adjust, argument);
#else
    reinterpret_cast<void (*)(unsigned char*, int)>(static_cast<__UINTPTR_TYPE__>(function))(self + adjust, argument);
#endif
}
// Same lookup for a slot called as (this + adjust, second, third, fourth float).
inline void call_virtual_pointer_pointer_int_float(unsigned char* self, unsigned slot, unsigned char* second,
                                                   int third, float fourth) {
    const unsigned char* entry = load_pointer(self + 72) + slot;
    const int adjust = load_signed_half(entry);
    const unsigned function = load_word(entry + 4);
#ifdef NORRATH_HOST_ARENA
    norrath_host_call_pointer_pointer_int_float(function, self + adjust, second, third, fourth);
#else
    reinterpret_cast<void (*)(unsigned char*, unsigned char*, int, float)>(static_cast<__UINTPTR_TYPE__>(function))(
        self + adjust, second, third, fourth);
#endif
}
}
