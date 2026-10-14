#pragma once
// Field access helpers for the w11-actor-c bodies. Objects are byte blocks addressed
// by offset. A pointer stored in an object is a 32-bit word. Host checks define W11_ACTOR_C_HOST and
// supply w11_actor_c_host_base so those words become offsets from a host base address.
#if defined(W11_ACTOR_C_HOST)
extern unsigned char* w11_actor_c_host_base;
typedef void* (*W11ActorCHostSlot)(void*);
extern W11ActorCHostSlot w11_actor_c_host_slots[];
typedef void* (*W11ActorCHostSlotInt)(void*, int);
extern W11ActorCHostSlotInt w11_actor_c_host_slots_int[];
typedef void* (*W11ActorCHostSlotIntPtr)(void*, int, const void*);
extern W11ActorCHostSlotIntPtr w11_actor_c_host_slots_int_ptr[];
typedef int (*W11ActorCHostSlotIntInt)(void*, int, int);
extern W11ActorCHostSlotIntInt w11_actor_c_host_slots_int_int[];
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

// A virtual slot of the table pointed to by object+72 is an 8-byte entry: a signed halfword adjustment at
// +0 and the function word at +4. The function is called with the object plus the adjustment and nothing else.
// Host checks keep an index into w11_actor_c_host_slots in the
// function word.
inline void* call_slot(void* object, unsigned slot_byte_offset) {
    const unsigned char* entry = link(object, 72) + slot_byte_offset;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W11_ACTOR_C_HOST)
    return w11_actor_c_host_slots[function](bytes(object) + adjustment);
#else
    return reinterpret_cast<void* (*)(void*)>(static_cast<__UINTPTR_TYPE__>(function))(bytes(object) + adjustment);
#endif
}
// The same slot called with one int argument after the adjusted object (the delete-style slots take flags).
inline void* call_slot_int(void* object, unsigned slot_byte_offset, int argument) {
    const unsigned char* entry = link(object, 72) + slot_byte_offset;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W11_ACTOR_C_HOST)
    return w11_actor_c_host_slots_int[function](bytes(object) + adjustment, argument);
#else
    return reinterpret_cast<void* (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(bytes(object) + adjustment, argument);
#endif
}
// The same slot called with an int and a pointer after the adjusted object.
inline void* call_slot_int_ptr(void* object, unsigned slot_byte_offset, int argument, const void* pointer) {
    const unsigned char* entry = link(object, 72) + slot_byte_offset;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W11_ACTOR_C_HOST)
    return w11_actor_c_host_slots_int_ptr[function](bytes(object) + adjustment, argument, pointer);
#else
    return reinterpret_cast<void* (*)(void*, int, const void*)>(static_cast<__UINTPTR_TYPE__>(function))(bytes(object) + adjustment, argument, pointer);
#endif
}
// The same slot called with two ints after the adjusted object; the result is an int. The table pointer is read
// from table_offset (72 for ordinary virtual calls).
inline int call_slot_int_int(void* object, unsigned slot_byte_offset, int first, int second, unsigned table_offset = 72) {
    const unsigned char* entry = link(object, table_offset) + slot_byte_offset;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W11_ACTOR_C_HOST)
    return w11_actor_c_host_slots_int_int[function](bytes(object) + adjustment, first, second);
#else
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(bytes(object) + adjustment, first, second);
#endif
}
}  // namespace w11c
