// getRankDescription__15SkillShieldBashPUsi at 0x002c7db0, 392 bytes. Arguments: a skill object, a destination of 16-bit characters, a rank.
// The object's table is the word at +96. Entries are 8 bytes: a signed halfword that adjusts the object, then
// the function address at +4. The class table names entry +88 evaluateRampAtRank__5Skillii, +72
// getRampDamage__5SkilliifiPiT5 and +120 tweekRank__5Skillii; the calls are reproduced as table calls because the
// class is not defined here.
// Observed: Entry +72 (getRampDamage in the class table) with ramp 0, the rank, 0.2, 0 and two integer
// addresses, then ramps 1, 2 and 3.  When ramp 3 is exactly 1: key "shieldbash_rankdesc1", float arguments the
// two integers, ramp 1, ramp 2.  Otherwise key "shieldbash_rankdesc2", arguments the two integers, ramp 3,
// ramp 1, ramp 2.
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
// Entry +72: ramp number, rank, a float, an integer and the addresses of two integer results.
static inline void rampDamage(unsigned char* object, int index, int rank, float scale, int flag, int* low, int* high) {
    typedef void (*Fn)(unsigned char*, int, int, float, int, int*, int*);
    unsigned char* entry = pointerAt(object + 96) + 72;
    reinterpret_cast<Fn>(pointerAt(entry + 4))(object + *reinterpret_cast<short*>(entry), index, rank, scale, flag, low, high);
}
static inline void emit(unsigned short* out, const char* key, float a = 0.0f, float b = 0.0f, float c = 0.0f,
                        float d = 0.0f, float e = 0.0f, float f = 0.0f, float g = 0.0f, float h = 0.0f) {
    unsigned short* format = textGetString__FPc(key);
    usprintf__FPUsT0ffffffff(out, format, a, b, c, d, e, f, g, h);
}

extern "C" void getRankDescription__15SkillShieldBashPUsi(unsigned char* skill, unsigned short* out, int rank) {
    int lo, hi;
    rampDamage(skill, 0, rank, 0.2f, 0, &lo, &hi);
    int r1 = ramp(skill, 1, rank);
    int r2 = ramp(skill, 2, rank);
    int r3 = ramp(skill, 3, rank);
    if (r3 == 1)
        emit(out, "shieldbash_rankdesc1", static_cast<float>(lo), static_cast<float>(hi), static_cast<float>(r1),
             static_cast<float>(r2));
    else
        emit(out, "shieldbash_rankdesc2", static_cast<float>(lo), static_cast<float>(hi), static_cast<float>(r3),
             static_cast<float>(r1), static_cast<float>(r2));
}
