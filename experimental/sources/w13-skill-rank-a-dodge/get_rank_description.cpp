// getRankDescription__10SkillDodgePUsi at 0x002ae718, 132 bytes. Arguments: a skill object, a
// destination of 16-bit characters, a rank. Observed order: the object's table (the word at +96) is
// read and its entry at +88 is called with the object adjusted by that entry's halfword, the integer
// 0 and the rank; the integer result is converted to float. The text for the key "dodge_rankdesc" is
// then fetched with textGetString and the destination is filled by usprintf with the converted
// value first and seven zero floats after it. The function returns nothing.
// The image's table for the class names that slot evaluateRampAtRank__5Skillii; the call is
// reproduced as a table call because the class is not defined here.
// Unknown: the text itself (never read here) and what the ramp number 0 selects.
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
namespace {
typedef int (*RampFn)(unsigned char* object, int ramp, int rank);
int evaluateRamp(unsigned char* object, int ramp, int rank) {
    unsigned char* entry = pointerAt(object + 96) + 88;
    short adjust = *reinterpret_cast<short*>(entry);
    return reinterpret_cast<RampFn>(pointerAt(entry + 4))(object + adjust, ramp, rank);
}
}  // namespace

extern "C" void getRankDescription__10SkillDodgePUsi(unsigned char* skill, unsigned short* out, int rank) {
    float value = static_cast<float>(evaluateRamp(skill, 0, rank));
    unsigned short* format = textGetString__FPc("dodge_rankdesc");
    usprintf__FPUsT0ffffffff(out, format, value, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
