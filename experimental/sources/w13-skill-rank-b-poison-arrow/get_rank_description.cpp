// Observed getRankDescription__16SkillPoisonArrowPUsi at 0x0029fe20, 356 bytes: arguments the Skill
// object, a UTF-16 output buffer and a rank; no result. It calls the Skill table entry at +88 with
// indices 0, 2 and 3. It uses the text for "poisonarrow_rankdesc1" when value 2 is exactly 1 and
// the text for "poisonarrow_rankdesc2" otherwise, with value 0, value 2 and value 3 as floats (the
// other five arguments zero).
// Inferred: the key names a printf-style format for the rank text on the skill menus and the floats
// are its numbers; the values are per-rank numbers answered by the Skill table entry at +88 (called
// with a ramp index and the rank).
// Unknown: what the format text contains, and what each ramp index and value means.
extern "C" {
const unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* to, const unsigned short* format, float a, float b, float c, float d,
                              float e, float f, float g, float h);
}

#ifdef W13_RANK_HOST
__UINTPTR_TYPE__ hostWordAddress(unsigned word);
#endif

namespace {
// The Skill object's word at +96 holds the address of a table of 8-byte entries: a halfword to add to the
// object pointer, then the function word at entry + 4. The words are 32-bit; the host check maps them.
inline __UINTPTR_TYPE__ widen(unsigned word) {
#ifdef W13_RANK_HOST
    return hostWordAddress(word);
#else
    return word;
#endif
}
inline const unsigned char* tableOf(const unsigned char* skill) {
    unsigned word;
    __builtin_memcpy(&word, skill + 96, sizeof word);
    return reinterpret_cast<const unsigned char*>(widen(word));
}
template <class F>
inline F entryFunction(const unsigned char* entry) {
    unsigned word;
    __builtin_memcpy(&word, entry + 4, sizeof word);
    return reinterpret_cast<F>(widen(word));
}
inline short entryDelta(const unsigned char* entry) {
    short delta;
    __builtin_memcpy(&delta, entry, sizeof delta);
    return delta;
}
// Table entry at +88: fn(object + delta, index, rank) returning an int.
inline int evalRamp(unsigned char* skill, int index, int rank) {
    const unsigned char* entry = tableOf(skill) + 88;
    return entryFunction<int (*)(void*, int, int)>(entry)(skill + entryDelta(entry), index, rank);
}
}

extern "C" void getRankDescription__16SkillPoisonArrowPUsi(void* self, unsigned short* out, int rank) {
    unsigned char* skill = static_cast<unsigned char*>(self);
    const int e0 = evalRamp(skill, 0, rank);
    const int e2 = evalRamp(skill, 2, rank);
    const int e3 = evalRamp(skill, 3, rank);
    if (e2 == 1) {
        const unsigned short* text = textGetString__FPc("poisonarrow_rankdesc1");
        usprintf__FPUsT0ffffffff(out, text, static_cast<float>(e0), static_cast<float>(e2),
            static_cast<float>(e3), 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f);
    } else {
        const unsigned short* text = textGetString__FPc("poisonarrow_rankdesc2");
        usprintf__FPUsT0ffffffff(out, text, static_cast<float>(e0), static_cast<float>(e2),
            static_cast<float>(e3), 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f);
    }
}
