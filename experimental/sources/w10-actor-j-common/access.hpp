// Shared accessors for the w10-actor-j sources. Offsets are bytes from the object start.
// On the target, 32-bit words hold addresses. A host contract defines W10_ACTOR_J_HOST, keeps
// those words as offsets from w10_actor_j_host_base, and routes table calls through
// w10_actor_j_host_call so a test can log them. Nothing here is an EE input by itself.
#ifndef W10_ACTOR_J_ACCESS_HPP
#define W10_ACTOR_J_ACCESS_HPP

struct Point3 { float x, y, z; };

#if defined(W10_ACTOR_J_HOST)
extern unsigned char* w10_actor_j_host_base;
unsigned w10_actor_j_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2);
#endif

namespace w10j {

inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }

inline unsigned char* from_word(unsigned value) {
#if defined(W10_ACTOR_J_HOST)
    return w10_actor_j_host_base + value;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
inline unsigned to_word(const void* pointer) {
#if defined(W10_ACTOR_J_HOST)
    return pointer ? static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - w10_actor_j_host_base) : 0u;
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
#endif
}

inline unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
inline int& sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
inline float& real(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
inline short& half(unsigned char* o, unsigned offset) { return *reinterpret_cast<short*>(o + offset); }
inline unsigned long long& dword(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned long long*>(o + offset); }

// A table entry is {signed halfword adjustment, padding, function word}; the table address is
// the word at object+72. These helpers call the entry at tableOffset on the adjusted object.
inline unsigned char* table_entry(unsigned char* o, unsigned tableOffset) { return from_word(word(o, 72)) + tableOffset; }

inline unsigned call_entry(unsigned char* o, unsigned tableOffset) {
    unsigned char* e = table_entry(o, tableOffset);
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W10_ACTOR_J_HOST)
    return w10_actor_j_host_call(fn, adjusted, 0, 0, 0);
#else
    return reinterpret_cast<unsigned (*)(void*)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted);
#endif
}
inline unsigned call_entry(unsigned char* o, unsigned tableOffset, unsigned long a1) {
    unsigned char* e = table_entry(o, tableOffset);
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W10_ACTOR_J_HOST)
    return w10_actor_j_host_call(fn, adjusted, 1, a1, 0);
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned long)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, a1);
#endif
}
inline unsigned call_entry(unsigned char* o, unsigned tableOffset, unsigned long a1, unsigned long a2) {
    unsigned char* e = table_entry(o, tableOffset);
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W10_ACTOR_J_HOST)
    return w10_actor_j_host_call(fn, adjusted, 2, a1, a2);
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned long, unsigned long)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, a1, a2);
#endif
}

// A random component in [-1, 1): twice (eRand * 2^-16) minus one. eRand must be supplied.
template <class Rand>
inline float unit_component(unsigned char* o, Rand rand) {
    float v = static_cast<float>(static_cast<int>(rand(o))) * bits(0x37800000u);
    return v + v - 1.0f;
}
template <class Rand>
inline void unit_ball(unsigned char* o, Point3& p, Rand rand) {
    do {
        p.x = unit_component(o, rand);
        p.y = unit_component(o, rand);
        p.z = unit_component(o, rand);
    } while (1.0f < p.x * p.x + p.y * p.y + p.z * p.z);
}

}  // namespace w10j

#endif
