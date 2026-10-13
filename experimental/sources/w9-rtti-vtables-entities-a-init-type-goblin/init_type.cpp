// Observed initType__6Goblinb at 0x0022c250, 1464 bytes. Arguments: the object and a flag
// (the callers pass 0). W below is the table of singles at 0x4818ac (W[i] is the word at
// 0x4818ac + 4 * i; see the view for the values).
// 1. Byte +156 becomes 0; the 64-bit word at +160 loses bits 33, 34 and 37.
// 2. The word at +972 (the variant) selects one setParameters call; its integer argument is
//    the flag xor 1 and its singles are, in order:
//      0: W0 W0 W0 W0 W1;               bit 33 set in +160.
//      1: 1.5 1.5 W2 1.5 1.5;           bit 33 set.
//      2: 2.0 W5 W6 W7 W5;              bit 37 set; the single W8 is stored at +100.
//      3: W4 W3 1.5 W3 1.5, after 25.0 was stored at +28 and W3 at +224;
//                                       bit 37 set; W8 stored at +100.
//      4: W9 W9 W10 W9 W9;              bit 33 set; then byte +156 = 3 when the word at +208
//                                       is 1, 2 when it is 2 or 3, otherwise unchanged.
//      5: W11 1.0 W12 1.0 W13;          bit 34 set; W14 stored at +104; byte +156 = 6.
//    Any other variant makes no call. The variant is read again afterwards.
// 3. Four halfwords at +1100, +1108, +1116 and +1124 (8-byte stride) become 0xffff.
// 4. Unless the variant is 4, the signed word at +208 selects a skill step and a final byte:
//      1: skills[12], tag 24, W15, byte +156 = 1;
//      2: skills[14], tag 22, W16, byte +156 = 3;
//      3: skills[13], tag 21, W17, byte +156 = 2;
//      4: skills[26], tag 23, W18, byte +156 = 4;
//      other: nothing more.
// The skill step: d = (int)worldDifficulty. Slot 12 of the table at offset 96 of the object
// that skills[n] points to is called with that object plus the table entry's halfword
// adjustment, then 0 and d; the int result is b. eRand(object) is rolled once.
// total = b + (int)((float)b * constant * ((float)(int)roll * 2^-16)). The first of the four
// entries at +1100 whose halfword is 0xffff or equals the tag is rewritten: halfword +0 =
// tag, +2 = b, +4 = (short)total, +6 = 0; the scan stops there.
// Unknown: what the variants, skills, constants and +28/+100/+104/+224 words are.
extern "C" {
unsigned eRand__10GameObject(void* self);
unsigned norrath_actor_set_parameters(void* object, float a, float b, float c, float d, float e, int flag);
extern float worldDifficulty;
extern unsigned skills[];
extern const unsigned norrath_rtti_entities_a_view_4818ac[19];
}

namespace {
typedef unsigned long long Quad __attribute__((may_alias));
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
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
float w(unsigned index) {
    union { unsigned bits; float value; } u = {norrath_rtti_entities_a_view_4818ac[index]};
    return u.value;
}

__attribute__((always_inline)) inline void skill_step(unsigned char* object, unsigned skill_index, unsigned constant_index,
                                                      int tag) {
    const int difficulty = static_cast<int>(worldDifficulty);
    unsigned char* skill = from_word(skills[skill_index]);
    const unsigned char* table = from_word(read_word(skill + 96)) + 96;
    const int base = call_skill(read_word(table + 4), skill + read_half(table), 0, difficulty);
    const float scaled_base = static_cast<float>(base) * w(constant_index);
    const float rolled = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * 1.52587890625e-05f;
    const int total = base + static_cast<int>(scaled_base * rolled);
    for (int i = 0; i < 4; ++i) {
        unsigned char* entry = object + 1100u + 8u * unsigned(i);
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

extern "C" void initType__6Goblinb(void* object_storage, bool flag) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    const int inverted = !flag;
    object[156] = 0;
    Quad* const bits = reinterpret_cast<Quad*>(object + 160);
    *bits = *bits & 0xffffffd9ffffffffull;
    switch (read_word(object + 972u)) {
    case 0u:
        (void)norrath_actor_set_parameters(object, w(0), w(0), w(0), w(0), w(1), inverted);
        *bits = *bits | 0x200000000ull;
        break;
    case 1u:
        (void)norrath_actor_set_parameters(object, 1.5f, 1.5f, w(2), 1.5f, 1.5f, inverted);
        *bits = *bits | 0x200000000ull;
        break;
    case 2u:
        (void)norrath_actor_set_parameters(object, 2.0f, w(5), w(6), w(7), w(5), inverted);
        *bits = *bits | 0x2000000000ull;
        write_word(object + 100u, norrath_rtti_entities_a_view_4818ac[8]);
        break;
    case 3u:
        write_word(object + 28u, 0x41c80000u);
        write_word(object + 224u, norrath_rtti_entities_a_view_4818ac[3]);
        (void)norrath_actor_set_parameters(object, w(4), w(3), 1.5f, w(3), 1.5f, inverted);
        *bits = *bits | 0x2000000000ull;
        write_word(object + 100u, norrath_rtti_entities_a_view_4818ac[8]);
        break;
    case 4u: {
        (void)norrath_actor_set_parameters(object, w(9), w(9), w(10), w(9), w(9), inverted);
        *bits = *bits | 0x200000000ull;
        const int type = static_cast<int>(read_word(object + 208u));
        if (type == 1) object[156] = 3;
        else if (type > 0 && type < 4) object[156] = 2;
        break;
    }
    case 5u:
        (void)norrath_actor_set_parameters(object, w(11), 1.0f, w(12), 1.0f, w(13), inverted);
        *bits = *bits | 0x400000000ull;
        write_word(object + 104u, norrath_rtti_entities_a_view_4818ac[14]);
        object[156] = 6;
        break;
    default:
        break;
    }
    for (unsigned i = 0; i < 4u; ++i) write_half(object + 1100u + 8u * i, 0xffffu);
    if (read_word(object + 972u) == 4u) return;
    const int type = static_cast<int>(read_word(object + 208u));
    if (type == 2) {
        skill_step(object, 14u, 16u, 22);
        object[156] = 3;
    } else if (type < 3) {
        if (type == 1) {
            skill_step(object, 12u, 15u, 24);
            object[156] = 1;
        }
    } else if (type == 3) {
        skill_step(object, 13u, 17u, 21);
        object[156] = 2;
    } else if (type == 4) {
        skill_step(object, 26u, 18u, 23);
        object[156] = 4;
    }
}
