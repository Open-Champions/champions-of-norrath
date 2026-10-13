// Host check for MC_ConvertTempPlayerToReal__Fi (source: ../w10-front-a-mc-convert-temp-player-to-real).
// Not an EE plan input. Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// Fails if the wrong record is copied, the copy is short or long, a neighbouring record is
// touched, or convertIndiciesToPointers__Fi is not called once with the same index afterwards.
#include "../w10-front-a-mc-convert-temp-player-to-real/mc_convert_temp_player_to_real.cpp"

#include <cstring>
#include <initializer_list>

typedef PlayerRecord Record;  // the source's 23984-byte record
extern "C" Record tempPlayerStruct[4] = {};
extern "C" Record player[4] = {};

static int calls, seen_index;
static unsigned char seen_first, seen_last;
extern "C" void convertIndiciesToPointers__Fi(int index) {
    ++calls;
    seen_index = index;
    // the copy must be complete before the conversion runs
    seen_first = player[index].bytes[0];
    seen_last = player[index].bytes[23983];
}

static unsigned char pattern(int record, int i) {
    return static_cast<unsigned char>((i * 31 + record * 17 + (i >> 8)) ^ 0x5a);
}

int main() {
    for (int n = 0; n < 4; ++n)
        for (int i = 0; i < 23984; ++i) {
            tempPlayerStruct[n].bytes[i] = pattern(n, i);
            player[n].bytes[i] = static_cast<unsigned char>(~pattern(n, i));
        }
    Record before[4];
    std::memcpy(before, player, sizeof player);
    for (int n : {2, 0, 3}) {
        calls = 0;
        MC_ConvertTempPlayerToReal__Fi(n);
        if (calls != 1 || seen_index != n) return 1;
        if (seen_first != pattern(n, 0) || seen_last != pattern(n, 23983)) return 2;
        if (std::memcmp(player[n].bytes, tempPlayerStruct[n].bytes, 23984) != 0) return 3;
        std::memcpy(before[n].bytes, player[n].bytes, 23984);
        for (int k = 0; k < 4; ++k)
            if (std::memcmp(before[k].bytes, player[k].bytes, 23984) != 0) return 4;
    }
    // record 1 was never selected: still the complemented pattern, and temp records are unchanged
    for (int i = 0; i < 23984; ++i) {
        if (player[1].bytes[i] != static_cast<unsigned char>(~pattern(1, i))) return 5;
        for (int k = 0; k < 4; ++k)
            if (tempPlayerStruct[k].bytes[i] != pattern(k, i)) return 6;
    }
    return 0;
}
