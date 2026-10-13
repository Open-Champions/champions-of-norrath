#include "modifier.cpp"
#include <cstdlib>
#include <cstring>
#include <set>
unsigned char ModifierCodes[8] = {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7};
struct Block {
    alignas(8) unsigned char state[512];
    alignas(8) unsigned char report[64];
};

static void fill(Block& b, int round) {
    for (unsigned char& c : b.state) c = static_cast<unsigned char>(rand());
    for (unsigned char& c : b.report) c = static_cast<unsigned char>(rand());
    int held = rand() % 6, now = rand() % 6;
    if (round % 7 == 0) { held = 0; }
    if (round % 11 == 0) { now = 0; }
    *reinterpret_cast<int*>(b.state + 256) = held;
    *reinterpret_cast<short*>(b.report + 16) = static_cast<short>(now);
    for (int i = 0; i < 8; ++i) { b.state[260 + i] = static_cast<unsigned char>(rand() % 12); b.report[20 + i] = static_cast<unsigned char>(rand() % 12); }
    unsigned queued = rand() % 13;
    if (round % 5 == 0) queued = 9;
    if (round % 9 == 0) queued = 10;
    *reinterpret_cast<unsigned short*>(b.state + 38) = static_cast<unsigned short>(queued);
}


static void push(unsigned char* state, const unsigned char* report, unsigned code, unsigned dir, bool* full) {
    unsigned short n = *reinterpret_cast<unsigned short*>(state + 38);
    if (n >= 10) { if (full) *full = true; return; }
    unsigned char* e = state + 40 + 16 * n;
    std::memcpy(e, report, 4);
    unsigned short v[4] = {static_cast<unsigned short>(code), report[10], report[11], static_cast<unsigned short>(dir)};
    std::memcpy(e + 4, &v[0], 2); std::memcpy(e + 8, &v[1], 2); std::memcpy(e + 10, &v[2], 2); std::memcpy(e + 12, &v[3], 2);
    *reinterpret_cast<unsigned short*>(state + 38) = n + 1;
}

static void model(unsigned char* state, const unsigned char* report) {
    for (int bit = 0; bit < 8; ++bit) {
        bool was = (state[33] >> bit) & 1, is = (report[11] >> bit) & 1;
        if (was == is) continue;
        int code = ModifierCodes[bit];
        if (is) state[224 + code / 8] |= static_cast<unsigned char>(1 << (code % 8));
        else state[224 + code / 8] &= static_cast<unsigned char>(~(1 << (code % 8)));
        push(state, report, code, is ? 1 : 2, nullptr);
    }
}

int main() {
    srand(21);
    for (int round = 0; round < 20000; ++round) {
        Block got, want;
        fill(got, round);
        std::memcpy(&want, &got, sizeof got);
        CheckforModifierKeyEvents(got.state, got.report);
        model(want.state, want.report);
        if (std::memcmp(&got, &want, sizeof got) != 0) return 1;

    }
    return 0;
}
