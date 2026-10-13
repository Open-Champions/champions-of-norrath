#pragma once
// Helpers for w10-actor-d bodies. Object fields hold 32-bit pointer words at their observed byte
// offsets. Host checks define W10_ACTOR_D_HOST: pointer words are then offsets from
// w10_actor_d_host_base and function words are indexes into w10_actor_d_host_functions, because a
// host pointer does not fit in a 32-bit object word.
#if defined(W10_ACTOR_D_HOST)
extern unsigned char* w10_actor_d_host_base;
typedef void (*W10ActorDHostFunction)(void*, int);
extern W10ActorDHostFunction w10_actor_d_host_functions[];
#endif

namespace w10_actor_d {
inline unsigned char* from_word(unsigned word) {
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_host_base + word;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
#endif
}
inline unsigned char* word_at(const unsigned char* base, unsigned offset) {
    return from_word(*reinterpret_cast<const unsigned*>(base + offset));
}

// A virtual slot of the table pointed to by object+72 is an 8-byte entry: a signed halfword
// adjustment at +0 and the function word at +4. The function is called with the object plus the
// adjustment and one int argument.
inline void call_slot(void* self, unsigned slot_byte_offset, int argument) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned char* entry = word_at(object, 72) + slot_byte_offset;
    const short adjustment = *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W10_ACTOR_D_HOST)
    w10_actor_d_host_functions[function](object + adjustment, argument);
#else
    reinterpret_cast<void (*)(void*, int)>(static_cast<__UINTPTR_TYPE__>(function))(object + adjustment, argument);
#endif
}
}  // namespace w10_actor_d
