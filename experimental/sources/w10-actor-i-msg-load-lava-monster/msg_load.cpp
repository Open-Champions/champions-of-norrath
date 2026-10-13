// Observed msg_load__11LavaMonsterPc at 0x001ed0a0, 348 bytes.
// Calls msg_load__8CreaturePc with the buffer, which returns the byte count n, and reads one word f
// after those n bytes (the word msg_save__11LavaMonsterPc wrote). Sets the type halfword of all four
// 8-byte records at object+1012 to -1. When f is nonzero: calls the skill object skills[13]'s table
// entry at +96 (the object's word at +96 points to a table; its entry at +96 is an adjustment halfword
// and a function word at +4) with the skill object adjusted, 0 and (int)worldDifficulty; the result is
// s. With r = eRand(object), the amount e = (int)((float)s * 0.2f * ((float)r / 65536)) (the 0.2 factor is
// the single 0x3e4ccccc). The first record whose type is -1 or 21 gets: type 21, halfword +2 = s,
// halfword +4 = s + e, halfword +6 = 0; later records are left alone. Returns n + 4.
// Inferred: the record is a timed effect, filled when the saved flag was set; the amounts scale with
// the difficulty roll. Unknown: the effect's meaning. Nothing here ran on the EE.
extern "C" {
extern float worldDifficulty;
extern unsigned skills[];
int msg_load__8CreaturePc(void* self, char* buffer);
int eRand__10GameObject(void* self);
}

extern "C" int msg_load__11LavaMonsterPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    const unsigned* in = reinterpret_cast<const unsigned*>(buffer + msg_load__8CreaturePc(self, buffer));
    const unsigned flag = *in++;
    for (int i = 3; i >= 0; --i) *reinterpret_cast<short*>(object + 1012 + 8 * i) = -1;
    if (flag != 0u) {
        unsigned char* skill = reinterpret_cast<unsigned char*>(skills[13]);
        unsigned char* entry = *reinterpret_cast<unsigned char**>(skill + 96) + 96;
        const int base = reinterpret_cast<int (*)(void*, int, int)>(*reinterpret_cast<unsigned*>(entry + 4))(
            skill + *reinterpret_cast<short*>(entry), 0, static_cast<int>(worldDifficulty));
        float scaled = static_cast<float>(base) * __builtin_bit_cast(float, 0x3e4ccccc);
        scaled *= static_cast<float>(eRand__10GameObject(object)) * __builtin_bit_cast(float, 0x37800000);
        const int extra = static_cast<int>(scaled);
        for (int i = 0; i < 4; ++i) {
            short* record = reinterpret_cast<short*>(object + 1012 + 8 * i);
            if (record[0] == -1 || record[0] == 21) {
                record[0] = 21;
                record[3] = 0;
                record[1] = static_cast<short>(base);
                record[2] = static_cast<short>(base + extra);
                break;
            }
        }
    }
    return static_cast<int>(reinterpret_cast<const char*>(in) - buffer);
}
