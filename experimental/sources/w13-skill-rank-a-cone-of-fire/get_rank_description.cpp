// getRankDescription__15SkillConeOfFirePUsi at 0x002b6788, 268 bytes. Arguments: a skill object, a destination of 16-bit characters, a rank.
// The object's table is the word at +96. Entries are 8 bytes: a signed halfword that adjusts the object, then
// the function address at +4. The class table names entry +88 evaluateRampAtRank__5Skillii, +72
// getRampDamage__5SkilliifiPiT5 and +120 tweekRank__5Skillii; the calls are reproduced as table calls because the
// class is not defined here.
// Observed: Ramps 0 and 1.  Float arguments: ramp 0, ramp 0 scaled and rounded, ramp 1.  Text key
// "coneoffire_rankdesc".
// The function returns nothing. All text keys are lookup strings passed to textGetString; the texts themselves were not read.
// Unknown: what each ramp number selects, and the meaning of the 0.2 and the rounding.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* pointerAt(const unsigned char* p) {
    unsigned word = p[0] | (p[1] << 8) | (p[2] << 16) | (static_cast<unsigned>(p[3]) << 24);
    return word == 0 ? nullptr : norrath_host_pointer(word);
}
#else
static inline unsigned char* pointerAt(const unsigned char* p) {
    return *reinterpret_cast<unsigned char* const*>(p);
}
#endif

extern "C" {
unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* destination, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
}
// Entry +88 of the object's table: the object adjusted by the entry's halfword, a ramp number, the rank.
static inline int ramp(unsigned char* object, int index, int rank) {
    typedef int (*Fn)(unsigned char*, int, int);
    unsigned char* entry = pointerAt(object + 96) + 88;
    return reinterpret_cast<Fn>(pointerAt(entry + 4))(object + *reinterpret_cast<short*>(entry), index, rank);
}
// v + v * 0.2f, then rounded half away from zero by adding or subtracting 0.5f and truncating.
static inline int rounded(int value) {
    float x = static_cast<float>(value);
    float y = x + x * 0.2f;
    return static_cast<int>(y >= 0.0f ? y + 0.5f : y - 0.5f);
}
static inline void emit(unsigned short* out, const char* key, float a = 0.0f, float b = 0.0f, float c = 0.0f,
                        float d = 0.0f, float e = 0.0f, float f = 0.0f, float g = 0.0f, float h = 0.0f) {
    unsigned short* format = textGetString__FPc(key);
    usprintf__FPUsT0ffffffff(out, format, a, b, c, d, e, f, g, h);
}

extern "C" void getRankDescription__15SkillConeOfFirePUsi(unsigned char* skill, unsigned short* out, int rank) {
    int r0 = ramp(skill, 0, rank);
    int r1 = ramp(skill, 1, rank);
    emit(out, "coneoffire_rankdesc", static_cast<float>(r0), static_cast<float>(rounded(r0)), static_cast<float>(r1));
}
