// Observed initAttackVariant__5Demon at 0x001e9ea8, 384 bytes.
// 1. Four halfwords at +1068, +1076, +1084 and +1092 (an 8-byte stride) become
//    0xffff. Byte +156 becomes 0.
// 2. The signed word at +208 chooses what happens next:
//      2 or 3: byte +156 = 8.
//      4:      byte +156 = 6.
//      1:      the skill step below, then byte +156 = 2.
//      other (zero, negative, above 4): nothing more.
// The skill step (word at +208 equal to 1): the float worldDifficulty is
// truncated to an int d. Slot 12 of the table at offset 96 of the object that
// skills[13] points to (skills[13] is skillFireWeapons in the image, a data
// symbol of the original) is called with that object plus the table entry's
// halfword adjustment, then 0 and d; the int result is b. A float
// (b * 0.2f, the word at 0x480bb8) is multiplied by (int)eRand(object) * 2^-16
// and truncated to an int e. Scanning the four entries from +1068, the first entry
// whose halfword is 0xffff or 21 is rewritten: halfword +0 = 21, +2 = b, +4 =
// (short)(b + e), +6 = 0; the scan stops there. If none matches, nothing is written.
// Inferred: the 8-byte entries are slots of 21-tagged effect records; no name is
// claimed. Unknown: what skill slot 12 returns and what the 0.2 factor means.
extern "C" {
unsigned eRand__10GameObject(void* self);
extern float worldDifficulty;
extern unsigned skills[];
extern const unsigned norrath_rtti_entities_a_view_480bb8;
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int read_half(const unsigned char* p) {
    const unsigned v = unsigned(p[0]) | (unsigned(p[1]) << 8);
    return v < 0x8000u ? static_cast<int>(v) : static_cast<int>(v) - 0x10000;
}
void write_half(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
}
#if defined(NORRATH_HOST_CHECK)
extern "C" unsigned char* norrath_host_pointer(unsigned word);
extern "C" int norrath_host_call_skill(unsigned word, void* self, int a, int b);
unsigned char* from_word(unsigned word) { return norrath_host_pointer(word); }
int call_skill(unsigned word, void* self, int a, int b) { return norrath_host_call_skill(word, self, a, b); }
#else
unsigned char* from_word(unsigned word) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word));
}
int call_skill(unsigned word, void* self, int a, int b) {
    typedef int (*SkillFn)(void* self, int a, int b);
    return reinterpret_cast<SkillFn>(static_cast<__UINTPTR_TYPE__>(word))(self, a, b);
}
#endif
float factor() {
    union { unsigned bits; float value; } u = {norrath_rtti_entities_a_view_480bb8};
    return u.value;
}
}

extern "C" void initAttackVariant__5Demon(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    for (unsigned i = 0; i < 4u; ++i) write_half(object + 1068u + 8u * i, 0xffffu);
    object[156] = 0;
    const int variant = static_cast<int>(read_word(object + 208u));
    if (variant < 4) {
        if (variant >= 2) {
            object[156] = 8;
        } else if (variant == 1) {
            const int difficulty = static_cast<int>(worldDifficulty);
            unsigned char* skill = from_word(skills[13]);
            const unsigned char* table = from_word(read_word(skill + 96)) + 96;
            const int base = call_skill(read_word(table + 4), skill + read_half(table), 0, difficulty);
            const float scaled = static_cast<float>(base) * factor();
            const float roll = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * 1.52587890625e-05f;
            const int extra = static_cast<int>(scaled * roll);
            for (int i = 0; i < 4; ++i) {
                unsigned char* entry = object + 1068u + 8u * unsigned(i);
                const int tag = read_half(entry);
                if (tag == -1 || tag == 21) {
                    write_half(entry, 21u);
                    write_half(entry + 6, 0u);
                    write_half(entry + 2, unsigned(base));
                    write_half(entry + 4, unsigned(base + extra));
                    break;
                }
            }
            object[156] = 2;
        }
    } else if (variant == 4) {
        object[156] = 6;
    }
}
