// Host contract for animInit__Fv. Not an EE plan input.
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>

extern "C" void animInit__Fv();
extern "C" alignas(8) unsigned char frameStates[150 * 3868 + 24] = {};
extern "C" alignas(8) unsigned char animResults[100 * 1568] = {};
extern "C" alignas(8) unsigned char identityResult[1600] = {};
extern "C" alignas(8) unsigned char table[8408 + 128 * 4 + 4] = {};
extern "C" unsigned char* fsList = nullptr;
extern "C" unsigned char* arList = nullptr;
extern "C" unsigned char* arListBack = nullptr;

namespace {
int failures = 0;

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}

unsigned low_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(pointer));
}

unsigned float_bits(float value) {
    union {
        float value;
        unsigned bits;
    } converted;
    converted.value = value;
    return converted.bits;
}
}

int main() {
    const auto frame_base = reinterpret_cast<std::uintptr_t>(frameStates);
    if ((frame_base >> 32) != ((frame_base + sizeof frameStates) >> 32)) {
        std::fprintf(stderr, "fail: frameStates crosses a 4GB boundary\n");
        return 1;
    }
    std::memset(frameStates, 0xa5, sizeof frameStates);
    std::memset(animResults, 0x5a, sizeof animResults);
    std::memset(identityResult, 0x3c, sizeof identityResult);
    std::memset(table, 0x11, sizeof table);
    fsList = nullptr;
    arList = nullptr;
    arListBack = nullptr;
    animInit__Fv();

    expect(fsList == frameStates, "fsList");
    expect(arList == animResults, "arList");
    expect(arListBack == animResults + 99 * 1568, "arListBack");
    for (int index = 0; index < 149; ++index) {
        const unsigned char* record = frameStates + index * 3868;
        expect(load_word(record + 12) == low_bits(frameStates + (index + 1) * 3868), "frame next");
        if (index == 0) expect(load_word(record + 16) == 0, "first frame previous");
        else expect(load_word(record + 16) == low_bits(frameStates + (index - 1) * 3868), "frame previous");
        expect(record[0] == 0, "frame body cleared");
    }
    expect(load_word(frameStates + 149 * 3868 + 12) == 0, "last frame next");
    expect(load_word(frameStates + 149 * 3868 + 16) == low_bits(frameStates + 148 * 3868 + 0x40),
           "last frame previous");
    for (int byte = 150 * 3868; byte < 150 * 3868 + 24; ++byte) {
        expect(frameStates[byte] == 0xa5, "frame tail");
    }

    for (int index = 0; index < 99; ++index) {
        const unsigned char* record = animResults + index * 1568;
        expect(load_word(record + 1556) == low_bits(animResults + (index + 1) * 1568), "result next");
        if (index == 0) expect(load_word(record + 1560) == 0, "first result previous");
        else expect(load_word(record + 1560) == low_bits(animResults + (index - 1) * 1568), "result previous");
        expect(record[0] == 0, "result body cleared");
    }
    expect(load_word(animResults + 99 * 1568 + 1556) == 0, "last result next");
    expect(load_word(animResults + 99 * 1568 + 1560) == low_bits(animResults + 98 * 1568), "last result previous");

    for (int byte = 0; byte < 4; ++byte) expect(identityResult[byte] == 0x3c, "identity head");
    expect(load_word(identityResult + 4) == 0x6dc18000u, "identity word");
    expect(identityResult[8] == 64 && identityResult[9] == 0, "identity half");
    for (int byte = 10; byte < 16; ++byte) expect(identityResult[byte] == 0x3c, "identity gap");
    for (int group = 0; group < 64; ++group) {
        const unsigned char* row = identityResult + 16 + group * 24;
        for (int half = 0; half < 12; ++half) {
            const unsigned value = static_cast<unsigned>(row[half * 2]) |
                                   (static_cast<unsigned>(row[half * 2 + 1]) << 8);
            const unsigned expected = (half == 0 || half == 5 || half == 10) ? 4096u : 0x3c3cu;
            expect(value == expected, "identity row");
        }
    }

    union {
        float value;
        unsigned bits;
    } base;
    base.bits = 0x3f879c7cu;
    for (int exponent = -64; exponent < 64; ++exponent) {
        const unsigned got = load_word(table + 8408 + (exponent + 64) * 4);
        const unsigned want = float_bits(powf(base.value, static_cast<float>(exponent)));
        expect(got == want, "pitch sample");
    }
    for (int byte = 0; byte < 16; ++byte) expect(table[byte] == 0x11, "table head");
    for (int byte = 0; byte < 4; ++byte) expect(table[8408 + 128 * 4 + byte] == 0x11, "table tail");

    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    return 0;
}
