// getRankDescription__10SkillShockPUsi at 0x0028c548, 296 bytes. Arguments: a skill object, a destination of 16-bit characters, a rank.
// The object's table is the word at +96. Entries are 8 bytes: a signed halfword that adjusts the object, then
// the function address at +4. The class table names entry +88 evaluateRampAtRank__5Skillii, +72
// getRampDamage__5SkilliifiPiT5 and +120 tweekRank__5Skillii; the calls are reproduced as table calls because the
// class is not defined here.
// Observed: Ramp 1 through entry +88, plus one: n.  Then entry +72 (getRampDamage in the class table) with
// ramp 0, the rank, 0.2, 0 and two integer addresses.  When n is at least 2: text key "shock_rankdesc2", float
// arguments the two integers and n.  Otherwise text key "shock_rankdesc1" and only the two integers.
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

extern "C" void getRankDescription__10SkillShockPUsi(unsigned char* skill, unsigned short* out, int rank) {
    int n = ramp(skill, 1, rank) + 1;
    int lo, hi;
    rampDamage(skill, 0, rank, 0.2f, 0, &lo, &hi);
    if (n >= 2)
        emit(out, "shock_rankdesc2", static_cast<float>(lo), static_cast<float>(hi), static_cast<float>(n));
    else
        emit(out, "shock_rankdesc1", static_cast<float>(lo), static_cast<float>(hi));
}
