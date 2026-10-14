#include "missile_weapon_ctor.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

unsigned char vt_10Projectile[16] __asm__("_vt$10Projectile");
unsigned char vt_13MissileWeapon[16] __asm__("_vt$13MissileWeapon");
int frameNm = 1000;
unsigned levelFlags = 0;

static std::vector<std::string> order;
static unsigned char* current;
static unsigned point_seen[3];
static void* map_seen;
static char** arguments_seen;
static unsigned flags_at_link, vtable_at_link, word76_at_link;
static void* link_args[3];
static unsigned char light_at_add[64];
static unsigned char light_byte8_at_add;
static void* deactivated;
static std::string sequence_name;
static std::string sequence_lump;
static void* sequence_object;
static float sequence_gain;
static int trail_args[8];
static float trail_float;
static int extend_trail;
static unsigned extend_point[3];
static unsigned word244_at_run_list;
static void* sound_header;
static void* sound_position;
static float sound_floats[3];
static int trail_result = 77;
static int sound_result = 5;

static unsigned word(const unsigned char* base, unsigned offset) { unsigned v; std::memcpy(&v, base + offset, 4); return v; }
static unsigned short half(const unsigned char* base, unsigned offset) { unsigned short v; std::memcpy(&v, base + offset, 2); return v; }
static float flt(const unsigned char* base, unsigned offset) { float v; std::memcpy(&v, base + offset, 4); return v; }
static void put_half(unsigned char* base, unsigned offset, short v) { std::memcpy(base + offset, &v, 2); }

extern "C" unsigned norrath_game_object_construct(void* object, const void* point, void* resource_map, char** arguments) {
    order.push_back("construct");
    std::memcpy(point_seen, point, 12);
    map_seen = resource_map;
    arguments_seen = arguments;
    unsigned flags = 0x00100000u;
    std::memcpy(static_cast<unsigned char*>(object) + 12, &flags, 4);
    unsigned vt = 0x1111u;
    std::memcpy(static_cast<unsigned char*>(object) + 72, &vt, 4);
    return 0;
}
extern "C" void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* object_head, void* value) {
    order.push_back("link");
    link_args[0] = head;
    link_args[1] = object_head;
    link_args[2] = value;
    flags_at_link = word(current, 12);
    vtable_at_link = word(current, 72);
    word76_at_link = word(current, 76);
    return nullptr;
}
extern "C" void norrath_light_add(void* light) {
    order.push_back("light");
    std::memcpy(light_at_add, light, sizeof light_at_add);
    light_byte8_at_add = static_cast<unsigned char*>(light)[8];
    if (light != current + 368) std::fprintf(stderr, "fail: light address\n");
}
extern "C" void lightDeactivate__FP5Light(void* light) { order.push_back("deactivate"); deactivated = light; }
extern "C" void* lumpFindResource__FPcT0(char* lump_name, char* resource_name) {
    order.push_back("lump");
    sequence_lump = lump_name;
    sequence_name = resource_name;
    return reinterpret_cast<void*>(0x5000);
}
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float gain) {
    order.push_back("sequence");
    if (header != reinterpret_cast<void*>(0x5000)) std::fprintf(stderr, "fail: header passed on\n");
    sequence_object = object;
    sequence_gain = gain;
    return 0;
}
extern "C" int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h) {
    order.push_back("trail");
    int args[8] = {a, b, 0, d, e, f, g, h};
    std::memcpy(trail_args, args, sizeof args);
    trail_float = c;
    return trail_result;
}
extern "C" void P_ExtendTrail__FiG6Point3(int trail, const void* point) {
    order.push_back("extend");
    extend_trail = trail;
    std::memcpy(extend_point, point, 12);
}
extern "C" void objectAddToRunList__FP10GameObject(void* object) {
    order.push_back("runlist");
    word244_at_run_list = word(static_cast<unsigned char*>(object), 244);
}
extern "C" int soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, void* position, float a, float b, float c) {
    order.push_back("sound");
    sound_header = header;
    sound_position = position;
    sound_floats[0] = a;
    sound_floats[1] = b;
    sound_floats[2] = c;
    return sound_result;
}

static void reset(unsigned char* object) {
    order.clear();
    std::memset(object, 0xcc, 512);
    deactivated = nullptr;
    sequence_name.clear();
    sound_header = nullptr;
}

int main() {
    int bad = 0;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); bad = 1; } };
    alignas(16) unsigned char object[512];
    current = object;
    const unsigned P[3] = {0x40a00000u, 0x40c00000u, 0x40e00000u};
    const unsigned V[3] = {0x3f800000u, 0x40000000u, 0x40400000u};
    char creature_storage[8], vag_a[4], vag_b[4], model[4], texture[4];
    unsigned char description[96];
    auto count = [&](const char* name) { int n = 0; for (auto& s : order) if (s == name) ++n; return n; };

    // Kind 0 with a creature, no description: trail starts.
    reset(object);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        object, 0, model, texture, P, V, 0x12345, creature_storage, vag_a, vag_b, 11, 12, 13, 14, 15, 16, nullptr);
    expect(std::memcmp(point_seen, P, 12) == 0 && map_seen == nullptr && arguments_seen == nullptr, "base constructor args");
    expect(vtable_at_link == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_10Projectile)), "base vtable stored before link");
    expect(flags_at_link == (0x00100000u | 0x4081u) && word76_at_link == 0u, "flags and head cleared before link");
    expect(link_args[0] == object + 76 && link_args[1] == creature_storage && link_args[2] == creature_storage, "link arguments");
    expect(word(object, 72) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vt_13MissileWeapon)), "final vtable");
    expect(object[80] == 0 && flt(object, 28) == 12.0f, "byte 80 and float 28");
    for (int k = 0; k < 5; ++k) expect(word(object, 92 + 24 * k) == 0u, "five cleared entry words");
    expect(word(object, 96) == 0xccccccccu && word(object, 88) == 0xccccccccu, "entry neighbours untouched");
    expect(word(object, 292) == 0u && std::memcmp(object + 416, P, 12) == 0, "word 292 and position copy");
    expect(light_byte8_at_add == 0 && light_at_add[9] == 0, "light bytes cleared before lightAdd");
    expect(half(light_at_add, 10) == 255 && half(light_at_add, 12) == 155 && half(light_at_add, 14) == 0 &&
               half(light_at_add, 16) == 0xffe0 && half(light_at_add, 18) == 255 && half(light_at_add, 20) == 155 &&
               half(light_at_add, 22) == 0 && half(light_at_add, 24) == 0xffe0, "light halfwords");
    expect(flt(light_at_add, 28) == 168.0f && flt(light_at_add, 36) == 168.0f && word(light_at_add, 32) == 0x38149b92u &&
               word(light_at_add, 40) == 0x38149b92u && word(light_at_add, 44) == 0u, "light floats");
    expect(object[40] == 72 && object[254] == 0 && half(object, 252) == 0 && half(object, 250) == 0, "bytes 40, 254, halves");
    expect(word(object, 208) == 0u, "kind stored");
    expect(deactivated == object + 368 && count("deactivate") == 1 && count("sequence") == 0, "kind 0 deactivates only");
    expect(count("trail") == 1 && trail_args[0] == 1 && trail_args[1] == 122 && trail_args[3] == 0 && trail_args[4] == 128 &&
               trail_args[5] == 128 && trail_args[6] == 128 && trail_args[7] == 32 && word(object, 364) == 77u,
           "trail started");
    expect(std::memcmp(&trail_float, "\xbe\x9f\x1a\x3e", 4) == 0, "trail float");
    expect(extend_trail == 77 && extend_point[0] == word(object, 16) && extend_point[1] == word(object, 20) &&
               extend_point[2] == word(object, 24), "trail extended at the object position");
    expect(word(object, 212) == 11u && word(object, 216) == 12u && word(object, 220) == 13u && word(object, 224) == 14u &&
               word(object, 228) == 15u && word(object, 232) == 16u, "six integers");
    expect(std::memcmp(object + 428, V, 12) == 0 && object[204] == 0 && object[248] == 1 && object[205] == 1, "V and bytes");
    expect(half(object, 56) == 0x2345, "short stored");
    expect(word(object, 444) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(model)) &&
               word(object, 448) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(texture)) && word(object, 296) == 0u,
           "model, texture, description");
    expect(word(object, 12) == (0x00100000u | 0x4081u | 0x4001u), "flags");
    expect(word(object, 236) == 1300u && word244_at_run_list == 0u && word(object, 244) == 0u, "frameNm + 300 and word 244");
    expect(sound_header == vag_a && sound_position == object + 16 && sound_floats[0] == 1.0f && sound_floats[1] == 1.0f &&
               sound_floats[2] == 0.0f && word(object, 240) == 5u, "first sound");
    expect(word(object, 440) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(vag_b)), "second sound header");
    const char* sequence[] = {"construct", "link", "light", "deactivate", "trail", "extend", "runlist", "sound"};
    expect(order.size() == 8, "eight calls");
    for (size_t i = 0; i < order.size() && i < 8; ++i) expect(order[i] == sequence[i], "call order");

    // Kinds that start sequences.
    const struct { int kind; const char* name; } kinds[] = {
        {2, "spel_cas_explarrow.anm"}, {1, "spel_cas_firearrow.anm"}, {5, "spel_cas_icearrow.anm"}, {6, "spel_cas_shkarrow.anm"}};
    for (auto& k : kinds) {
        reset(object);
        __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
            object, k.kind, model, texture, P, V, 0, nullptr, nullptr, nullptr, 1, 2, 3, 4, 5, 6, nullptr);
        expect(sequence_name == k.name && sequence_lump == "fx.lmp" && sequence_object == object && sequence_gain == 1.0f,
               "sequence name");
        expect(count("link") == 0, "no link without a creature");
        expect(deactivated == nullptr && count("trail") == 0, "no deactivate or trail");
        expect(sound_header == nullptr && word(object, 240) == 0u && word(object, 440) == 0u, "no sound header");
        if (k.kind >= 5) {
            expect(half(object, 380) == 155 && half(object, 382) == 255 && half(object, 386) == 0 && half(object, 378) == 0 &&
                       half(object, 388) == 155 && half(object, 390) == 255, "light recolour for kinds 5 and 6");
        } else {
            expect(half(object, 380) == 155 && half(object, 382) == 0 && half(object, 386) == 255 && half(object, 378) == 255 &&
                       half(object, 388) == 155 && half(object, 390) == 0, "base light colours stay for other kinds");
        }
    }
    // Kind 3 deactivates and plays nothing.
    reset(object);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        object, 3, model, texture, P, V, 0, nullptr, nullptr, nullptr, 1, 2, 3, 4, 5, 6, nullptr);
    expect(deactivated == object + 368 && sequence_name.empty() && count("trail") == 0, "kind 3");
    // Kind 4 does nothing special.
    reset(object);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        object, 4, model, texture, P, V, 0, nullptr, nullptr, nullptr, 1, 2, 3, 4, 5, 6, nullptr);
    expect(deactivated == nullptr && sequence_name.empty() && count("trail") == 0, "kind 4");

    // Description effects.
    auto run_with_description = [&](int key0, short value0, int key1, short value1, int slot_for_second) {
        std::memset(description, 0, sizeof description);
        unsigned nonzero = 1;
        std::memcpy(description, &nonzero, 4);
        for (int i = 0; i < 4; ++i) put_half(description, 36 + 8 * i, 99);
        put_half(description, 36, static_cast<short>(key0));
        put_half(description, 38, value0);
        if (key1 != 0) {
            put_half(description, 36 + 8 * slot_for_second, static_cast<short>(key1));
            put_half(description, 38 + 8 * slot_for_second, value1);
        }
        reset(object);
        __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
            object, 0, model, texture, P, V, 0, nullptr, nullptr, nullptr, 1, 2, 3, 4, 5, 6, description);
    };
    run_with_description(24, 3, 0, 0, 0);
    expect(count("trail") == 0 && word(object, 364) == 0u, "effect 24 with a value blocks the trail and clears +364");
    run_with_description(22, -4, 0, 0, 0);
    expect(count("trail") == 0 && word(object, 364) == 0u, "effect 22 negative value counts");
    run_with_description(21, 1, 0, 0, 0);
    expect(count("trail") == 0 && word(object, 364) == 0u, "effect 21 counts");
    run_with_description(24, 0, 21, 2, 3);
    expect(count("trail") == 0 && word(object, 364) == 0u, "zero-valued 24 falls through to a later key in the fourth slot");
    run_with_description(24, 0, 0, 0, 0);
    expect(count("trail") == 1 && word(object, 364) == 77u, "a zero value does not block the trail");
    run_with_description(50, 9, 0, 0, 0);
    expect(count("trail") == 1, "unrelated key does not block");
    expect(word(object, 296) == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(description)), "description stored");
    // A zero first word skips the search entirely.
    std::memset(description, 0, sizeof description);
    put_half(description, 36, 24);
    put_half(description, 38, 5);
    reset(object);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        object, 0, model, texture, P, V, 0, nullptr, nullptr, nullptr, 1, 2, 3, 4, 5, 6, description);
    expect(count("trail") == 1, "zero first word skips the effect search");
    // levelFlags bit 0x100 suppresses the trail.
    levelFlags = 0x100;
    reset(object);
    __13MissileWeaponiP12_modelHeaderP8_textureG6Point3T4iP8CreatureP10_vagHeaderT8iiiiiiPQ24item11description(
        object, 0, model, texture, P, V, 0, nullptr, nullptr, nullptr, 1, 2, 3, 4, 5, 6, nullptr);
    expect(count("trail") == 0, "levelFlags bit suppresses the trail");
    levelFlags = 0;
    if (bad) return 1;
    std::puts("missile weapon constructor contract passed");
    return 0;
}
