// Observed getRankDescription__9SkillBashPUsi at 0x002a5138, 392 bytes: arguments the Skill object,
// a UTF-16 output buffer and a rank; no result. It calls the Skill table entry at +72 with ramp
// index 0 and the rank (scale 0.2, zero, two int outputs), then the entry at +88 with indices 1, 2
// and 3. When value 3 is exactly 1 it uses the text for "bash_rankdesc1" with the two outputs,
// value 1 and value 2 as floats; otherwise the text for "bash_rankdesc2" with the two outputs,
// value 3, value 1 and value 2.
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
// Table entry at +72: fn(object + delta, index, rank, 0.2f, 0, &first, &second), two ints through pointers.
inline void rampPair(unsigned char* skill, int index, int rank, int* first, int* second) {
    const unsigned char* entry = tableOf(skill) + 72;
    entryFunction<void (*)(void*, int, int, float, int, int*, int*)>(entry)(skill + entryDelta(entry), index, rank, 0.2f, 0,
                                                                              first, second);
}
}

extern "C" void getRankDescription__9SkillBashPUsi(void* self, unsigned short* out, int rank) {
    unsigned char* skill = static_cast<unsigned char*>(self);
    int a, b;
    rampPair(skill, 0, rank, &a, &b);
    const int e1 = evalRamp(skill, 1, rank);
    const int e2 = evalRamp(skill, 2, rank);
    const int e3 = evalRamp(skill, 3, rank);
    if (e3 == 1) {
        const unsigned short* text = textGetString__FPc("bash_rankdesc1");
        usprintf__FPUsT0ffffffff(out, text, static_cast<float>(a), static_cast<float>(b),
            static_cast<float>(e1), static_cast<float>(e2), 0.0f,
            0.0f, 0.0f, 0.0f);
    } else {
        const unsigned short* text = textGetString__FPc("bash_rankdesc2");
        usprintf__FPUsT0ffffffff(out, text, static_cast<float>(a), static_cast<float>(b),
            static_cast<float>(e3), static_cast<float>(e1), static_cast<float>(e2),
            0.0f, 0.0f, 0.0f);
    }
}
