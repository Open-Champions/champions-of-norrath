#pragma once
// Field access helpers for the w11-actor-c bodies. Objects are byte blocks addressed
// by offset. A pointer stored in an object is a 32-bit word. Host checks define W11_ACTOR_C_HOST and
// supply w11_actor_c_host_base so those words become offsets from a host base address.
#if defined(W11_ACTOR_C_HOST)
extern unsigned char* w11_actor_c_host_base;
#endif
namespace w11c {
typedef unsigned __attribute__((may_alias)) Word;
typedef unsigned short __attribute__((may_alias)) Half;
typedef short __attribute__((may_alias)) SHalf;
typedef float __attribute__((may_alias)) Float;
typedef unsigned long long __attribute__((may_alias)) Double;

inline unsigned char* bytes(void* object) { return static_cast<unsigned char*>(object); }
inline Word& word(void* object, unsigned offset) { return *reinterpret_cast<Word*>(bytes(object) + offset); }
inline Half& half(void* object, unsigned offset) { return *reinterpret_cast<Half*>(bytes(object) + offset); }
inline SHalf& shalf(void* object, unsigned offset) { return *reinterpret_cast<SHalf*>(bytes(object) + offset); }
inline Float& real(void* object, unsigned offset) { return *reinterpret_cast<Float*>(bytes(object) + offset); }
inline signed char& sbyte(void* object, unsigned offset) { return *reinterpret_cast<signed char*>(bytes(object) + offset); }
inline unsigned char& ubyte(void* object, unsigned offset) { return bytes(object)[offset]; }

inline unsigned char* from_word(unsigned value) {
#if defined(W11_ACTOR_C_HOST)
    return value == 0u ? nullptr : w11_actor_c_host_base + value;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
inline unsigned to_word(const void* pointer) {
#if defined(W11_ACTOR_C_HOST)
    return pointer == nullptr ? 0u : static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - w11_actor_c_host_base);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}
// The pointer stored at object+offset.
inline unsigned char* link(void* object, unsigned offset) { return from_word(word(object, offset)); }
}  // namespace w11c
