#include "player_init.cpp"
#include <cstdio>
#include <cstring>

// Four records plus a sentinel tail so a write past the record is visible.
alignas(8) unsigned char player[4 * 23984 + 64];
int fadeTimer[28];
int fadeIconY[28];
int abilityTable[20];

static int order[8];
static int steps;
static int weightIndex = -99, equipIndex = -99;
static int classAtWeight, classAtEquip;
static char nameAtWeight[16];
static unsigned char* recordForCalls;

extern "C" void playerRecomputeWeight__Fi(int i) {
    order[steps++] = 1;
    weightIndex = i;
    classAtWeight = *reinterpret_cast<int*>(recordForCalls + 80);
    std::memcpy(nameAtWeight, recordForCalls, 8);
}
extern "C" void playerRecomputeAbilityModifiers__Fv() { order[steps++] = 2; }
extern "C" void setupDefaultEquipment__Fi(int i) {
    order[steps++] = 3;
    equipIndex = i;
    classAtEquip = *reinterpret_cast<int*>(recordForCalls + 80);
}

// Independent description of the expected record, written as a table of
// (offset, width, value) stores applied to a zero image instead of as code.
struct Store { unsigned offset; unsigned width; unsigned value; };
static void apply(unsigned char* image, const Store& s) {
    for (unsigned i = 0; i < s.width; ++i) image[s.offset + i] = static_cast<unsigned char>(s.value >> (8 * i));
}
static unsigned bits(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }

static void expected_record(unsigned char* image, int second, const char* name, signed char b0,
                            signed char b1, signed char b2, signed char b3, const signed char* tint,
                            const int* ability) {
    std::memset(image, 0, 23984);
    static const Store fixed[] = {
        {23982, 1, 0xaa}, {231, 1, 0xaa}, {168, 1, 0xaa},
        {36, 1, 'N'}, {37, 1, 'o'}, {38, 1, 'n'}, {39, 1, 'e'}, {40, 1, 0},
        {200, 4, 0x41200000}, {196, 4, 0x41200000}, {232, 4, 500}, {188, 4, 0x3d999999},
        {184, 4, 0x3d999999}, {312, 4, 0x42fa0000}, {242, 1, 1}, {208, 4, 11}, {216, 4, 2},
        {212, 4, 1}, {220, 4, 1}, {89, 1, 1}, {88, 1, 1}, {21868, 4, 0xffffffffu}, {23582, 1, 5},
        {23581, 1, 1},
    };
    for (const Store& s : fixed) apply(image, s);
    for (unsigned w = 0; w < 64; ++w) apply(image, {23712 + w * 4, 4, 0xffffffffu});
    for (unsigned e = 0; e < 15; ++e) apply(image, {22008 + e * 104, 2, 0xffff});
    int kind;
    switch (second) {
        case 0: case 1: kind = 0; break;
        case 8: case 9: kind = 2; break;
        case 4: case 5: kind = 1; break;
        case 6: case 7: kind = 4; break;
        default: kind = 3; break;  // 2, 3 and every other value
    }
    apply(image, {84, 4, static_cast<unsigned>(second)});
    apply(image, {80, 4, static_cast<unsigned>(kind)});
    for (unsigned i = 0; i < 4; ++i) {
        const int word = tint ? tint[i] : ability[kind * 4 + i];
        apply(image, {120 + i * 4, 4, static_cast<unsigned>(word)});
    }
    std::strcpy(reinterpret_cast<char*>(image), name);
    image[32] = static_cast<unsigned char>(b0);
    image[33] = static_cast<unsigned char>(b1);
    image[34] = static_cast<unsigned char>(b2);
    image[35] = static_cast<unsigned char>(b3);
    static const float length[5] = {16.0f, 35.0f, 25.0f, 21.0f, 40.0f};
    apply(image, {172, 4, bits(53.0f)});
    apply(image, {196, 4, bits(length[kind])});
}

static int run(int index, int second, const char* name, signed char b0, signed char b1, signed char b2,
               signed char b3, const signed char* tint) {
    recordForCalls = player + index * 23984;
    std::memset(player, 0x5a, sizeof player);
    std::memset(fadeTimer, 0x77, sizeof fadeTimer);
    std::memset(fadeIconY, 0x77, sizeof fadeIconY);
    steps = 0;
    weightIndex = equipIndex = -99;
    unsigned char before[sizeof player];
    std::memcpy(before, player, sizeof player);
    playerInit__FiiPcScScScScT2(index, second, const_cast<char*>(name), b0, b1, b2, b3,
                                const_cast<char*>(reinterpret_cast<const char*>(tint)));

    unsigned char want[23984];
    expected_record(want, second, name, b0, b1, b2, b3, tint, abilityTable);
    if (std::memcmp(player + index * 23984, want, 23984) != 0) {
        for (unsigned i = 0; i < 23984; ++i)
            if (player[index * 23984 + i] != want[i]) {
                std::fprintf(stderr, "byte %u: got %02x want %02x\n", i, player[index * 23984 + i], want[i]);
                break;
            }
        return 1;
    }
    // Nothing outside the record changed.
    for (unsigned i = 0; i < sizeof player; ++i) {
        const bool inside = i >= static_cast<unsigned>(index) * 23984u && i < static_cast<unsigned>(index + 1) * 23984u;
        if (!inside && player[i] != before[i]) return 2;
    }
    // Only this record's row of both fade tables is cleared.
    for (int i = 0; i < 28; ++i) {
        const bool row = i >= index * 7 && i < index * 7 + 7;
        if (fadeTimer[i] != (row ? 0 : 0x77777777) || fadeIconY[i] != (row ? 0 : 0x77777777)) return 3;
    }
    if (steps != 3 || order[0] != 1 || order[1] != 2 || order[2] != 3) return 4;
    if (weightIndex != index || equipIndex != index) return 5;
    // The weight callee runs before the name is copied and after the class word is set.
    int kindWanted = 0;
    std::memcpy(&kindWanted, want + 80, 4);
    if (classAtWeight != kindWanted || classAtEquip != kindWanted) return 6;
    if (nameAtWeight[0] != 0) return 7;
    return 0;
}

int main() {
    for (int i = 0; i < 20; ++i) abilityTable[i] = 100 + i * 7;
    const signed char tint[4] = {-3, 0, 120, -128};
    int rc;
    for (int second = -3; second <= 12; ++second) {
        for (int index = 0; index < 4; ++index) {
            if ((rc = run(index, second, "Toonces", 5, 1, -1, 0, 0))) {
                std::fprintf(stderr, "second %d index %d null tint rc %d\n", second, index, rc);
                return 10 + rc;
            }
        }
        if ((rc = run(2, second, "Edna", -2, 3, 9, -7, tint))) {
            std::fprintf(stderr, "second %d tint rc %d\n", second, rc);
            return 30 + rc;
        }
    }
    return 0;
}
