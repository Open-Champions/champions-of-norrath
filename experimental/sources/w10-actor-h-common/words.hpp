#pragma once
// Helpers for objects that hold 32-bit pointer words (the target is a 32-bit machine).
// Host checks define W10_ACTOR_H_HOST: words then become offsets from w10h_host_base and function
// words become indexes resolved by w10h_host_fn, so field offsets stay the real ones.
#if defined(W10_ACTOR_H_HOST)
extern unsigned char* w10h_host_base;
void* w10h_host_fn(unsigned word);
#endif

namespace w10h {

inline unsigned char* from_word(unsigned w) {
#if defined(W10_ACTOR_H_HOST)
    return w10h_host_base + w;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(w));
#endif
}

template <class F>
inline F fn_from_word(unsigned w) {
#if defined(W10_ACTOR_H_HOST)
    return reinterpret_cast<F>(w10h_host_fn(w));
#else
    return reinterpret_cast<F>(static_cast<__UINTPTR_TYPE__>(w));
#endif
}

inline unsigned& word_at(void* object, unsigned offset) {
    return *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(object) + offset);
}

// Class table of an object: the pointer word stored at object+72.
inline unsigned char* table_of(void* object) { return from_word(word_at(object, 72)); }

// The word at table+4 is a function taking no arguments that returns the class's type node;
// every caller here compares that node with a class's own __tf node.
inline void* type_node_of(void* object) {
    return fn_from_word<void* (*)()>(word_at(table_of(object), 4))();
}

// Table entry at table+offset: a signed halfword adjustment, then (at +4) a function word.
// The function is called with the object plus the adjustment as first argument.
inline short entry_adjust(void* object, unsigned offset) {
    return *reinterpret_cast<short*>(table_of(object) + offset);
}
inline unsigned entry_function(void* object, unsigned offset) {
    return word_at(table_of(object) + offset, 4);
}
inline void* adjusted(void* object, unsigned offset) {
    return static_cast<unsigned char*>(object) + entry_adjust(object, offset);
}

inline void copy_bytes(void* to, const void* from, unsigned count) {
    unsigned char* d = static_cast<unsigned char*>(to);
    const unsigned char* s = static_cast<const unsigned char*>(from);
    for (unsigned i = 0; i < count; ++i) d[i] = s[i];
}

}  // namespace w10h
