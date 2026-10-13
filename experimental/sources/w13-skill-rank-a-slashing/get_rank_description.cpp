// getRankDescription__13SkillSlashingPUsi at 0x00313bb0, 120 bytes. Arguments: a skill object, a destination of 16-bit characters, a rank.
// The object is never read.
// Observed: No table call and the object is not read.  value = 5 * rank + 10 for a rank above 0, otherwise 0.
// Text key "SlashingDesc"; the value is the first float argument.
// The function returns nothing. All text keys are lookup strings passed to textGetString; the texts themselves were not read.
extern "C" {
unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* destination, unsigned short* format, float a, float b, float c,
                              float d, float e, float f, float g, float h);
}
static inline void emit(unsigned short* out, const char* key, float a = 0.0f, float b = 0.0f, float c = 0.0f,
                        float d = 0.0f, float e = 0.0f, float f = 0.0f, float g = 0.0f, float h = 0.0f) {
    unsigned short* format = textGetString__FPc(key);
    usprintf__FPUsT0ffffffff(out, format, a, b, c, d, e, f, g, h);
}

extern "C" void getRankDescription__13SkillSlashingPUsi(unsigned char*, unsigned short* out, int rank) {
    int value = 0;
    if (rank > 0) value = rank * 5 + 10;
    emit(out, "SlashingDesc", static_cast<float>(value));
}
