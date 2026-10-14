// Observed initAttackVariant__8Froglock at 0x001ffb90, 636 bytes.
// 1. Four halfwords at +1068, +1076, +1084 and +1092 (8-byte stride) become 0xffff
//    and byte +156 becomes 0.
// 2. When the word at +960 is 17, the signed word at +208 decides:
//      1: skill step with skills[26], tag 23, constant at 0x4810b4; byte +156 = 4.
//      2: skill step with skills[14], tag 22, constant at 0x4810b8; byte +156 = 3.
//      other: nothing more.
//    When the word at +960 is not 17: variant 1 gives byte +156 = 4, 2 gives 3,
//    3 or 4 gives 2, anything else leaves it 0. No skill step.
// The skill step: d = (int)worldDifficulty. Slot 12 of the table at offset 96 of the
// object that skills[n] points to is called with that object plus the table
// entry's halfword adjustment, then 0 and d; the int result is b. eRand(object)
// is rolled once. With f = (float)b: total = (int)(f + f * constant *
// ((float)(int)roll * 2^-16)). The first of the four entries at +1068 whose
// halfword is 0xffff or equals the tag is rewritten: halfword +0 = tag, +2 = b, +4 =
// (short)total, +6 = 0; the scan stops there; no match writes nothing.
// Unknown: what skill slot 12 returns, what the 0.2 constants mean, and what the
// skills[14] and skills[26] objects are called in the original.
extern "C" {
unsigned eRand__10GameObject(void* self);
extern float worldDifficulty;
extern unsigned skills[];
extern const unsigned norrath_rtti_entities_a_view_4810b4;
extern const unsigned norrath_rtti_entities_a_view_4810b8;
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
float as_single(unsigned bits) {
    union { unsigned bits; float value; } u = {bits};
    return u.value;
}

__attribute__((always_inline)) inline void skill_step(unsigned char* object, unsigned skill_index, unsigned constant_bits, int tag) {
    const int difficulty = static_cast<int>(worldDifficulty);
    unsigned char* skill = from_word(skills[skill_index]);
    const unsigned char* table = from_word(read_word(skill + 96)) + 96;
    const int base = call_skill(read_word(table + 4), skill + read_half(table), 0, difficulty);
    const float constant = as_single(constant_bits);
    const float rolled = static_cast<float>(static_cast<int>(eRand__10GameObject(object)));
    const float whole = static_cast<float>(base);
    const float scaled = rolled * 1.52587890625e-05f;
    const float bonus = whole * constant * scaled;
    const int total = static_cast<int>(whole + bonus);
    for (int i = 0; i < 4; ++i) {
        unsigned char* entry = object + 1068u + 8u * unsigned(i);
        const int current = read_half(entry);
        if (current == -1 || current == tag) {
            write_half(entry, unsigned(tag));
            write_half(entry + 6, 0u);
            write_half(entry + 2, unsigned(base));
            write_half(entry + 4, unsigned(total));
            break;
        }
    }
}
}

extern "C" void initAttackVariant__8Froglock(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    for (unsigned i = 0; i < 4u; ++i) write_half(object + 1068u + 8u * i, 0xffffu);
    const unsigned mode = read_word(object + 960u);
    object[156] = 0;
    const int variant = static_cast<int>(read_word(object + 208u));
    if (mode == 17u) {
        if (variant == 1) {
            skill_step(object, 26u, norrath_rtti_entities_a_view_4810b4, 23);
            object[156] = 4;
        } else if (variant == 2) {
            skill_step(object, 14u, norrath_rtti_entities_a_view_4810b8, 22);
            object[156] = 3;
        }
    } else if (variant == 2) {
        object[156] = 3;
    } else if (variant == 1) {
        object[156] = 4;
    } else if (variant == 3 || variant == 4) {
        object[156] = 2;
    }
}
