// Shared accessors for the w13-skill-cast-a sources. Offsets are bytes from the object start.
// On the target, 32-bit words hold addresses. A host contract defines W13_SKILL_CAST_A_HOST, keeps those words
// as offsets from w13_skill_cast_a_host_base and routes the table call through w13_skill_cast_a_host_call so a test
// can log it. Nothing here is an EE input by itself.
#ifndef W13_SKILL_CAST_A_SKILL_ACCESS_HPP
#define W13_SKILL_CAST_A_SKILL_ACCESS_HPP

struct Point3 { float x, y, z; };

#if defined(W13_SKILL_CAST_A_HOST)
extern unsigned char* w13_skill_cast_a_host_base;
int w13_skill_cast_a_host_call(unsigned function_word, void* adjusted_skill, int rank, int constant);
#endif

namespace w13sca {

inline unsigned char* from_word(unsigned value) {
#if defined(W13_SKILL_CAST_A_HOST)
    return w13_skill_cast_a_host_base + value;
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
#if defined(W13_SKILL_CAST_A_HOST)
    return w13_skill_cast_a_host_call(function, adjusted, a1, a2);
#else
    return reinterpret_cast<int (*)(void*, int, int)>(static_cast<__UINTPTR_TYPE__>(function))(adjusted, a1, a2);
#endif
}

}  // namespace w13sca

#endif
