// Shared accessors for the w13-skill-cast-b sources. Offsets are bytes from the object start.
// On the target, 32-bit words hold addresses. A host contract defines W13_SKILL_CAST_B_HOST, keeps those words as
// offsets from w13_skill_cast_b_host_base and routes the table call through w13_skill_cast_b_host_call so a test can
// log it. Nothing here is an EE input by itself.
#ifndef W13_SKILL_CAST_B_SKILL_ACCESS_HPP
#define W13_SKILL_CAST_B_SKILL_ACCESS_HPP

struct Point3 { float x, y, z; };

#if defined(W13_SKILL_CAST_B_HOST)
extern unsigned char* w13_skill_cast_b_host_base;
int w13_skill_cast_b_host_call(unsigned function_word, void* adjusted_skill, int rank, int constant);
int w13_skill_cast_b_host_call_object(unsigned function_word, void* adjusted_object, void* argument);
void* w13_skill_cast_b_host_call_object0(unsigned function_word, void* adjusted_object);
int w13_skill_cast_b_host_call_ramp(unsigned function_word, void* adjusted_skill, int index, int rank, float scale, int zero, int* low, int* high);
#endif

namespace w13scb {

inline unsigned char* from_word(unsigned value) {
#if defined(W13_SKILL_CAST_B_HOST)
    return value ? w13_skill_cast_b_host_base + value : nullptr;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}

// A skill's table address is the word at skill+96. A table entry is a signed halfword that adjusts the receiver,
// two bytes of padding, and a function word. This calls the entry at tableOffset with (adjusted skill, a1, a2).
inline int call_skill_entry(unsigned char* skill, unsigned tableOffset, int a1, int a2) {
    const unsigned char* entry = from_word(*reinterpret_cast<unsigned*>(skill + 96)) + tableOffset;
    unsigned char* adjusted = skill + *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W13_SKILL_CAST_B_HOST)
    return w13_skill_cast_b_host_call(function, adjusted, a1, a2);
#else
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, a1, a2);
#endif
}

// Calls the skill table entry at tableOffset as (adjusted skill, index, rank, scale, zero, low, high) and returns its
// integer result. This is the shape of getRampDamage__5SkilliifiPiT5 (slot at byte 72).
inline int call_ramp_damage(unsigned char* skill, unsigned tableOffset, int index, int rank, float scale, int zero, int* low, int* high) {
    const unsigned char* entry = from_word(*reinterpret_cast<unsigned*>(skill + 96)) + tableOffset;
    unsigned char* adjusted = skill + *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W13_SKILL_CAST_B_HOST)
    return w13_skill_cast_b_host_call_ramp(function, adjusted, index, rank, scale, zero, low, high);
#else
    return reinterpret_cast<int (*)(void*, int, int, float, int, int*, int*)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, index, rank, scale, zero, low, high);
#endif
}

// An object's table address is the word at object+tableField. This calls the entry at tableOffset (same entry
// layout) with (adjusted object, argument) and returns the integer result.
inline int call_object_entry(unsigned char* object, unsigned tableField, unsigned tableOffset, void* argument) {
    const unsigned char* entry = from_word(*reinterpret_cast<unsigned*>(object + tableField)) + tableOffset;
    unsigned char* adjusted = object + *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W13_SKILL_CAST_B_HOST)
    return w13_skill_cast_b_host_call_object(function, adjusted, argument);
#else
    return reinterpret_cast<int (*)(void*, void*)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, argument);
#endif
}

// Same entry layout, called with only the adjusted object; returns the pointer result.
inline void* call_object_entry0(unsigned char* object, unsigned tableField, unsigned tableOffset) {
    const unsigned char* entry = from_word(*reinterpret_cast<unsigned*>(object + tableField)) + tableOffset;
    unsigned char* adjusted = object + *reinterpret_cast<const short*>(entry);
    const unsigned function = *reinterpret_cast<const unsigned*>(entry + 4);
#if defined(W13_SKILL_CAST_B_HOST)
    return w13_skill_cast_b_host_call_object0(function, adjusted);
#else
    return reinterpret_cast<void* (*)(void*)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted);
#endif
}

}  // namespace w13scb

#endif
