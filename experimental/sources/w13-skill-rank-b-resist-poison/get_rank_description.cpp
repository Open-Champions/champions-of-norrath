// Observed getRankDescription__17SkillResistPoisonPUsi at 0x00313ca0, 112 bytes: arguments the
// Skill object, a UTF-16 output buffer and a rank; no result. The object is not used. It looks up
// the text for "damagePerSecond" and passes it with the buffer to usprintf__FPUsT0ffffffff; the
// first float argument is the rank times five and the other seven are zero. The symbol is weak in
// the image.
// Inferred: the text reports damage per second, and five per rank is the per-second figure.
// Unknown: what the format text contains.
extern "C" {
const unsigned short* textGetString__FPc(const char* key);
void usprintf__FPUsT0ffffffff(unsigned short* to, const unsigned short* format, float a, float b, float c, float d,
                              float e, float f, float g, float h);
}

extern "C" void getRankDescription__17SkillResistPoisonPUsi(void* self, unsigned short* out, int rank) {
    (void)self;
    const unsigned short* text = textGetString__FPc("damagePerSecond");
    usprintf__FPUsT0ffffffff(out, text, static_cast<float>(rank * 5), 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}
