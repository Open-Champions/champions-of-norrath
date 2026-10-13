// Observed getRankDescription__9SkillRootPUsi at 0x002acf10, 260 bytes: arguments the Skill object,
// a UTF-16 output buffer and a rank; no result. It calls the Skill table entry at +88 with ramp
// indices 0, 1 and 2 (with the rank), looks up the text for "root_rankdesc", then calls
// getEnemyWord__Fi with value 0 and passes the buffer, the text and the floats value 0, the enemy
// word, value 1 and value 2 to usprintf__FPUsT0ffffffff (the other four arguments zero).
// Inferred: the key names a printf-style format for the rank text on the skill menus and the floats
// are its numbers; the values are per-rank numbers answered by the Skill table entry at +88 (called
// with a ramp index and the rank). getEnemyWord__Fi is called with the first value; its result is a
// word shown in the text.
// Unknown: what the format text contains, and what each ramp index and value means. What
// getEnemyWord__Fi returns for each argument.
extern "C" {
const unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* to, const unsigned short* format, float a, float b, float c, float d,
                              float e, float f, float g, float h);
int getEnemyWord__Fi(int value);
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

extern "C" void getRankDescription__9SkillRootPUsi(void* self, unsigned short* out, int rank) {
    unsigned char* skill = static_cast<unsigned char*>(self);
    const int e0 = evalRamp(skill, 0, rank);
    const int e1 = evalRamp(skill, 1, rank);
    const int e2 = evalRamp(skill, 2, rank);
    const unsigned short* text = textGetString__FPc("root_rankdesc");
    const int enemy = getEnemyWord__Fi(e0);
    usprintf__FPUsT0ffffffff(out, text, static_cast<float>(e0), static_cast<float>(enemy), static_cast<float>(e1),
        static_cast<float>(e2), 0.0f, 0.0f, 0.0f, 0.0f);
}
