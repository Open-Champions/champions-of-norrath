// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror -DW12_HUD_HOST host_check.cpp
// The 32-bit pointer words in the item are handles that hostWordAddress maps to host buffers.
#include "item_equipped.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>

extern "C" { alignas(8) unsigned char player[4 * 23984 + 64]; }
alignas(8) static unsigned char descs[4][64];
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    return word >= 0x5000u && word < 0x5004u ? reinterpret_cast<__UINTPTR_TYPE__>(descs[word - 0x5000u]) : 0;
}
static void setWord(int rec, int off, int w) { std::memcpy(player + rec * 23984 + off, &w, 4); }

int main() {
    int bad = 0;
    alignas(8) unsigned char item[16] = {0};
    const int word = static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(item));
    auto call = [&](int rec) { return itemEquipped__FiPQ24item11description(rec, item); };
    auto reset = [&](int desc) {
        std::memset(player, 0, sizeof player);
        std::memset(descs, 0, sizeof descs);
        std::memcpy(item, &desc, 4);
    };
    bad |= itemEquipped__FiPQ24item11description(0, nullptr) != 0;
    reset(0); setWord(0, 21896, word); bad |= call(0) != 0;  // null description
    // Kind A: byte 29 nonzero, or byte 28 equal to 8 -> the sixteen words at 21896 count.
    for (int mode = 0; mode < 3; ++mode) {
        reset(0x5000);
        if (mode == 0) descs[0][29] = 1;
        if (mode == 1) descs[0][28] = 8;
        if (mode == 2) { descs[0][29] = 0xff; descs[0][28] = 3; }
        for (int rec : {0, 2})
            for (int s = 0; s < 16; ++s) {
                std::memset(player, 0, sizeof player);
                setWord(rec, 21896 + s * 4, word);
                bad |= call(rec) != 1;
                bad |= call(rec == 0 ? 1 : 0) != 0;
            }
        std::memset(player, 0, sizeof player);
        setWord(0, 21896 + 16 * 4, word);  // one past the range
        setWord(0, 21960 + 4, word);       // the other kind's range does not count here
        bad |= call(0) != 0;
        bad |= call(0) != 0;
    }
    // Kind B: neither condition -> only the three words at 21960 count.
    reset(0x5001); descs[1][28] = 7; descs[1][29] = 0;
    for (int s = 0; s < 3; ++s) {
        std::memset(player, 0, sizeof player);
        setWord(1, 21960 + s * 4, word);
        bad |= call(1) != 1;
        bad |= call(0) != 0;
    }
    std::memset(player, 0, sizeof player);
    setWord(1, 21960 + 3 * 4, word); setWord(1, 21896, word); setWord(1, 21956, word);
    bad |= call(1) != 0;
    std::printf(bad ? "FAIL\n" : "ok: equipped test\n");
    return bad;
}
