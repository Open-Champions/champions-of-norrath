// getRankDescription__13SkillEntanglePUsi at 0x002c6e50, 260 bytes. Arguments: a skill object, a destination of 16-bit characters, a rank.
// The object's table is the word at +96. Entries are 8 bytes: a signed halfword that adjusts the object, then
// the function address at +4. The class table names entry +88 evaluateRampAtRank__5Skillii, +72
// getRampDamage__5SkilliifiPiT5 and +120 tweekRank__5Skillii; the calls are reproduced as table calls because the
// class is not defined here.
// Observed: Ramps 0, 1 and 2, then the text key "entangle_rankdesc", then getEnemyWord__Fi (an import of this
// package) with ramp 0.  Float arguments: ramp 0, the getEnemyWord result, ramp 1, ramp 2.
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
int getEnemyWord__Fi(int value);
}
// Entry +88 of the object's table: the object adjusted by the entry's halfword, a ramp number, the rank.
static inline int ramp(unsigned char* object, int index, int rank) {
    typedef int (*Fn)(unsigned char*, int, int);
    unsigned char* entry = pointerAt(object + 96) + 88;
    return reinterpret_cast<Fn>(pointerAt(entry + 4))(object + *reinterpret_cast<short*>(entry), index, rank);
}

extern "C" void getRankDescription__13SkillEntanglePUsi(unsigned char* skill, unsigned short* out, int rank) {
    int r0 = ramp(skill, 0, rank);
    int r1 = ramp(skill, 1, rank);
    int r2 = ramp(skill, 2, rank);
    unsigned short* format = textGetString__FPc("entangle_rankdesc");
    int word = getEnemyWord__Fi(r0);
    usprintf__FPUsT0ffffffff(out, format, static_cast<float>(r0), static_cast<float>(word), static_cast<float>(r1),
                             static_cast<float>(r2), 0.0f, 0.0f, 0.0f, 0.0f);
}
