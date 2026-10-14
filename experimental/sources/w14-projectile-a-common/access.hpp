// Shared accessors for the w14-projectile-a sources. Offsets are bytes from the object start.
// On the target, 32-bit words hold addresses. A host contract defines W14_PROJECTILE_A_HOST, keeps
// those words as offsets from w14_projectile_a_host_base, and routes table calls through
// w14_projectile_a_host_call so a test can log them. Nothing here is an EE input by itself.
#ifndef W14_PROJECTILE_A_ACCESS_HPP
#define W14_PROJECTILE_A_ACCESS_HPP

struct Point3 { float x, y, z; };

#if defined(W14_PROJECTILE_A_HOST)
extern unsigned char* w14_projectile_a_host_base;
void w14_projectile_a_host_hurt(unsigned function_word, void* adjusted_object, float amount, void* source, int flags);
unsigned w14_projectile_a_host_call(unsigned function_word, void* adjusted_object, int argument_count, unsigned long a1, unsigned long a2);
int w14_projectile_a_host_call3(unsigned function_word, void* adjusted_object, int a1, int a2, void* a3);
#endif

namespace w14pa {

inline float bits(unsigned value) { return __builtin_bit_cast(float, value); }

inline unsigned char* from_word(unsigned value) {
#if defined(W14_PROJECTILE_A_HOST)
    return value ? w14_projectile_a_host_base + value : nullptr;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
inline unsigned to_word(const void* pointer) {
#if defined(W14_PROJECTILE_A_HOST)
    return pointer ? static_cast<unsigned>(static_cast<const unsigned char*>(pointer) - w14_projectile_a_host_base) : 0u;
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
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call(fn, adjusted, 0, 0, 0);
#else
    return reinterpret_cast<unsigned (*)(void*)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted);
#endif
}
inline unsigned call_entry(unsigned char* o, unsigned tableOffset, unsigned long a1) {
    unsigned char* e = table_entry(o, tableOffset);
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call(fn, adjusted, 1, a1, 0);
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned long)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, a1);
#endif
}
inline unsigned call_entry(unsigned char* o, unsigned tableOffset, unsigned long a1, unsigned long a2) {
    unsigned char* e = table_entry(o, tableOffset);
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call(fn, adjusted, 2, a1, a2);
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned long, unsigned long)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, a1, a2);
#endif
}

// Calls a function word with no arguments and no object adjustment, returning a word.
inline unsigned call_word(unsigned fn) {
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call(fn, nullptr, 0, 0, 0);
#else
    return reinterpret_cast<unsigned (*)()>(static_cast<__UINTPTR_TYPE__>(fn))();
#endif
}

// Same, for a table whose address is the word at tableWordOffset instead of +72.
inline unsigned call_entry_from(unsigned char* o, unsigned tableWordOffset, unsigned tableOffset, unsigned long a1, unsigned long a2) {
    unsigned char* e = from_word(word(o, tableWordOffset)) + tableOffset;
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call(fn, adjusted, 2, a1, a2);
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned long, unsigned long)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, a1, a2);
#endif
}

// Calls the entry at table byte 16 (a hurt-style slot) on o with a float amount, a source object and flags.
inline void call_hurt(unsigned char* o, float amount, void* source, int flags) {
    unsigned char* e = table_entry(o, 16);
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W14_PROJECTILE_A_HOST)
    w14_projectile_a_host_hurt(fn, adjusted, amount, source, flags);
#else
    reinterpret_cast<void (*)(void*, float, void*, int)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, amount, source, flags);
#endif
}

// Same, with three arguments after the adjusted object.
inline unsigned call_entry_from(unsigned char* o, unsigned tableWordOffset, unsigned tableOffset, unsigned long a1, unsigned long a2, unsigned long a3) {
    unsigned char* e = from_word(word(o, tableWordOffset)) + tableOffset;
    unsigned char* adjusted = o + *reinterpret_cast<short*>(e);
    const unsigned fn = *reinterpret_cast<unsigned*>(e + 4);
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call(fn, adjusted, 3, a1, a2 + a3 * 0);
#else
    return reinterpret_cast<unsigned (*)(void*, unsigned long, unsigned long, unsigned long)>(static_cast<__UINTPTR_TYPE__>(fn))(adjusted, a1, a2, a3);
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

// 1.0 / sqrt(v): the EE rsqrt.s on the target (fs / sqrt(ft), as the original executes), a plain division on a host.
inline float inverse_length(float squared) {
#if defined(__mips__) && !defined(W14_PROJECTILE_A_HOST)
    float result;
    __asm__("rsqrt.s %0,%1,%2" : "=f"(result) : "f"(1.0f), "f"(squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(squared);
#endif
}

// The 80-byte item description a caller builds on its stack before gameAttackResolver (the layout the w11-actor-c sparks collision records
// use): bytes 32 and 33 are -1, the eight halfwords at 16..31 are 0 and the first halfword of each of the four 8-byte entries at 36, 44,
// 52 and 60 is -1. Bytes outside those ranges are not written.
inline void init_description(unsigned char* d) {
    for (int entry = 3; entry >= 0; --entry) *reinterpret_cast<short*>(d + 36 + 8 * entry) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int i = 0; i < 8; ++i) *reinterpret_cast<short*>(d + 16 + 2 * i) = 0;
}

// Finds the first of the four entries whose first halfword is -1 or `kind` and fills it in: first halfword `kind`, then `a` at +2, `b` at +4
// and `c` at +6. Nothing is written when no entry matches.
inline void fill_description(unsigned char* d, short kind, short a, short b, short c) {
    for (int entry = 0; entry < 4; ++entry) {
        unsigned char* e = d + 36 + 8 * entry;
        const short first = *reinterpret_cast<short*>(e);
        if (first == -1 || first == kind) {
            *reinterpret_cast<short*>(e) = kind;
            *reinterpret_cast<short*>(e + 2) = a;
            *reinterpret_cast<short*>(e + 4) = b;
            *reinterpret_cast<short*>(e + 6) = c;
            return;
        }
    }
}

// The call the hit handlers make on the object whose address is the word at o+96 of `object`: its table is the word at +96 (not +72), the
// entry at offset 112 is called on `object` (adjusted by the entry's signed halfword) with (0, halfword, address).
inline int call_entry_112(unsigned char* object, int halfword, void* address) {
    unsigned char* const entry = from_word(word(object, 96)) + 112;
    const unsigned function = *reinterpret_cast<unsigned*>(entry + 4);
    unsigned char* const adjusted = object + *reinterpret_cast<short*>(entry);
#if defined(W14_PROJECTILE_A_HOST)
    return w14_projectile_a_host_call3(function, adjusted, 0, halfword, address);
#else
    return reinterpret_cast<int (*)(void*, int, int, void*)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, 0, halfword, address);
#endif
}

}  // namespace w14pa

#endif
