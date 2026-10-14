#include "combine_deadkey.cpp"
#include <cstring>

static unsigned char run(int pending, unsigned char c) {
    alignas(16) unsigned char keyboard[640] = {};
    std::memcpy(keyboard + 528, &pending, 4);
    return CombineDeadkey__10VIKeyboardUc(keyboard, c);
}

int main() {
    if (run(94, 'a') != 0xe2 || run(94, 'U') != 0xdb) return 1;
    if (run(96, 'e') != 0xe8 || run(96, 'A') != 0xc0) return 2;
    if (run(180, 'y') != 0xfd || run(180, 'E') != 0xc9) return 3;
    if (run(168, 'u') != 0xfc || run(168, 'y') != 0xff || run(168, 'Y') != 0xdf) return 4;
    if (run(126, 'n') != 0xf1 || run(126, 'O') != 0xd5) return 5;
    if (run(94, 'y') != 0 || run(96, 'y') != 0 || run(126, 'e') != 0) return 6;
    if (run(0, 'a') != 0 || run(95, 'a') != 0 || run(-1, 'a') != 0) return 7;
    // each table has the documented number of nonzero entries
    const int expected[5][2] = {{94, 10}, {96, 10}, {180, 12}, {168, 12}, {126, 6}};
    for (const auto& e : expected) {
        int count = 0;
        for (int c = 0; c < 256; ++c) count += run(e[0], static_cast<unsigned char>(c)) != 0;
        if (count != e[1]) return 8;
    }
    return 0;
}
