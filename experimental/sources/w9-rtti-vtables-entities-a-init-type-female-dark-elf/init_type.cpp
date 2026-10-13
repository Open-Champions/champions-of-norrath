// Observed initType__13FemaleDarkElfb at 0x002853a8, 864 bytes. Arguments: the object
// and a flag (the callers pass 0).
// 1. Byte +156 becomes 0. The signed byte at +960 selects the setParameters call, whose
//    last (integer) argument is the flag xor 1:
//      +960 == 1: five singles 1.0, 1.2 (0x482424), 1.0, 1.0, 1.0; then byte +156 = 6;
//      otherwise: 1.4 (0x482428), 1.2 (0x48242c), 1.0, 1.0, 1.0.
// 2. Four halfwords at +1068, +1076, +1084 and +1092 (8-byte stride) become 0xffff.
// 3. The signed word at +208 selects a skill step and the final byte at +156:
//      1: skills[26], tag 23, constant 0x482430, byte +156 = 4;
//      2: skills[14], tag 22, constant 0x482434, byte +156 = 3;
//      3: skills[25], tag 38, constant 0x482438, byte +156 = 7;
//      other: nothing more (byte +156 keeps the value from step 1).
// The skill step: d = (int)worldDifficulty. Slot 12 of the table at offset 96 of the
// object that skills[n] points to is called with that object plus the table entry's
// halfword adjustment, then 0 and d; the int result is b. eRand(object) is rolled once.
// total = b + (int)((float)b * constant * ((float)(int)roll * 2^-16)). The first of the
// four entries at +1068 whose halfword is 0xffff or equals the tag is rewritten:
// halfword +0 = tag, +2 = b, +4 = (short)total, +6 = 0; the scan stops there.
// Unknown: what the skills are called, what the constants mean, and the field names.
extern "C" {
unsigned eRand__10GameObject(void* self);
unsigned norrath_actor_set_parameters(void* object, float a, float b, float c, float d, float e, int flag);
extern float worldDifficulty;
extern unsigned skills[];
extern const unsigned norrath_rtti_entities_a_view_482424;
extern const unsigned norrath_rtti_entities_a_view_482428;
extern const unsigned norrath_rtti_entities_a_view_48242c;
extern const unsigned norrath_rtti_entities_a_view_482430;
extern const unsigned norrath_rtti_entities_a_view_482434;
extern const unsigned norrath_rtti_entities_a_view_482438;
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

__attribute__((always_inline)) inline void skill_step(unsigned char* object, unsigned skill_index,
                                                      unsigned constant_bits, int tag) {
    const int difficulty = static_cast<int>(worldDifficulty);
    unsigned char* skill = from_word(skills[skill_index]);
    const unsigned char* table = from_word(read_word(skill + 96)) + 96;
    const int base = call_skill(read_word(table + 4), skill + read_half(table), 0, difficulty);
    const float scaled_base = static_cast<float>(base) * as_single(constant_bits);
    const float rolled = static_cast<float>(static_cast<int>(eRand__10GameObject(object))) * 1.52587890625e-05f;
    const int total = base + static_cast<int>(scaled_base * rolled);
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

extern "C" void initType__13FemaleDarkElfb(void* object_storage, bool flag) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    object[156] = 0;
    if (static_cast<signed char>(object[960]) == 1) {
        (void)norrath_actor_set_parameters(object, 1.0f, as_single(norrath_rtti_entities_a_view_482424), 1.0f, 1.0f,
                                           1.0f, !flag);
        object[156] = 6;
    } else {
        (void)norrath_actor_set_parameters(object, as_single(norrath_rtti_entities_a_view_482428),
                                           as_single(norrath_rtti_entities_a_view_48242c), 1.0f, 1.0f, 1.0f, !flag);
    }
    for (unsigned i = 0; i < 4u; ++i) write_half(object + 1068u + 8u * i, 0xffffu);
    const int variant = static_cast<int>(read_word(object + 208u));
    if (variant == 2) {
        skill_step(object, 14u, norrath_rtti_entities_a_view_482434, 22);
        object[156] = 3;
    } else if (variant == 1) {
        skill_step(object, 26u, norrath_rtti_entities_a_view_482430, 23);
        object[156] = 4;
    } else if (variant == 3) {
        skill_step(object, 25u, norrath_rtti_entities_a_view_482438, 38);
        object[156] = 7;
    }
}
