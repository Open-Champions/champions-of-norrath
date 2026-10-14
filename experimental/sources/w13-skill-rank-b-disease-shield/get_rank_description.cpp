// Observed getRankDescription__18SkillDiseaseShieldPUsi at 0x00296480, 488 bytes: arguments the
// Skill object, a UTF-16 output buffer and a rank; no result. It calls the Skill table entry at +88
// with indices 0, 1 and 2. It uses the text for "diseaseshield_rankdesc1" when value 2 is exactly 1
// and "diseaseshield_rankdesc2" otherwise, with value 0 as a float, value 0 plus a fifth of itself
// rounded (halves away from zero) as a float, then value 1 and value 2 as floats (the other four
// arguments zero).
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
// Float to nearest int, halves away from zero: add 0.5 (or subtract it when negative), then truncate.
inline int roundInt(float value) { return static_cast<int>(value >= 0.0f ? value + 0.5f : value - 0.5f); }
// The value plus a fifth of itself, rounded.
inline int plusFifth(float x) { return roundInt(x + x * 0.2f); }
}

extern "C" void getRankDescription__18SkillDiseaseShieldPUsi(void* self, unsigned short* out, int rank) {
    unsigned char* skill = static_cast<unsigned char*>(self);
    const int e0 = evalRamp(skill, 0, rank);
    const int e1 = evalRamp(skill, 1, rank);
    const int e2 = evalRamp(skill, 2, rank);
    if (e2 == 1) {
        const unsigned short* text = textGetString__FPc("diseaseshield_rankdesc1");
        usprintf__FPUsT0ffffffff(out, text, static_cast<float>(e0), static_cast<float>(plusFifth(static_cast<float>(e0))),
            static_cast<float>(e1), static_cast<float>(e2), 0.0f,
            0.0f, 0.0f, 0.0f);
    } else {
        const unsigned short* text = textGetString__FPc("diseaseshield_rankdesc2");
        usprintf__FPUsT0ffffffff(out, text, static_cast<float>(e0), static_cast<float>(plusFifth(static_cast<float>(e0))),
            static_cast<float>(e1), static_cast<float>(e2), 0.0f,
            0.0f, 0.0f, 0.0f);
    }
}
