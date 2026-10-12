#include "pad_big_rumble.cpp"

#include <cstdio>
#include <cstring>

int padVibrationEnable;
signed char padIndexMap[4];
unsigned rumphole[24];

static float read_float(unsigned word) {
    float value;
    std::memcpy(&value, &word, sizeof value);
    return value;
}
static unsigned bits(float value) {
    unsigned word;
    std::memcpy(&word, &value, sizeof word);
    return word;
}
static void reset() {
    for (unsigned& word : rumphole) word = 0xa5a5a5a5u;
    for (int index = 0; index < 4; ++index) padIndexMap[index] = static_cast<signed char>(index);
    padVibrationEnable = 1;
}
static bool untouched_except(int record, bool changed) {
    for (int word = 0; word < 24; ++word) {
        const bool in_record = word / 6 == record;
        if (in_record && changed && word % 6 >= 1 && word % 6 <= 3) continue;
        if (rumphole[word] != 0xa5a5a5a5u) return false;
    }
    return true;
}

int main() {
    reset();
    padVibrationEnable = 0;
    padBigRumble__Fiff(0, 1.0f, 0.5f);
    if (!untouched_except(0, false)) return 1;

    reset();
    rumphole[1] = 10;
    padBigRumble__Fiff(0, 0.5f, 0.6f);
    if (rumphole[1] != 30u) return 2;
    if (rumphole[2] != bits(0.6f)) return 3;
    if (read_float(rumphole[3]) != 0.6f / 30.0f) return 4;
    if (!untouched_except(0, true) || rumphole[0] != 0xa5a5a5a5u || rumphole[4] != 0xa5a5a5a5u) return 5;

    // An existing value that is not smaller leaves the record alone.
    reset();
    rumphole[1] = 30;
    padBigRumble__Fiff(0, 0.5f, 0.6f);
    if (rumphole[2] != 0xa5a5a5a5u || rumphole[3] != 0xa5a5a5a5u || rumphole[1] != 30u) return 6;
    reset();
    rumphole[1] = 31;
    padBigRumble__Fiff(0, 0.5f, 0.6f);
    if (rumphole[1] != 31u || rumphole[2] != 0xa5a5a5a5u) return 7;

    // The index map selects the record, and the strength is clamped to 0..1.
    reset();
    padIndexMap[2] = 3;
    rumphole[3 * 6 + 1] = 0;
    padBigRumble__Fiff(2, 1.0f, 4.0f);
    if (rumphole[3 * 6 + 1] != 60u || rumphole[3 * 6 + 2] != bits(1.0f)) return 8;
    if (read_float(rumphole[3 * 6 + 3]) != 1.0f / 60.0f) return 9;
    reset();
    rumphole[1] = 0;
    padBigRumble__Fiff(0, 1.0f, -2.0f);
    if (rumphole[2] != bits(0.0f) || read_float(rumphole[3]) != 0.0f) return 10;

    // Truncation toward zero, and a negative stored value is signed-less than any positive one.
    reset();
    rumphole[1] = 0xffffffffu;
    padBigRumble__Fiff(0, 0.02f, 0.25f);
    if (rumphole[1] != 1u) return 11;
    std::puts("pad big rumble passed");
    return 0;
}
