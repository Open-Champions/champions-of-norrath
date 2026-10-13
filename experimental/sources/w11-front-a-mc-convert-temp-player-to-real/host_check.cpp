// Host check for MC_ConvertTempPlayerToReal__Fi (source: ../w10-front-a-mc-convert-temp-player-to-real).
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// The two arrays are defined here with three records each; the check fails on a wrong record
// size, wrong record index, a copy of the wrong direction or a missing/misordered conversion call.
#include "../w10-front-a-mc-convert-temp-player-to-real/mc_convert_temp_player_to_real.cpp"

#include <cstdio>
#include <cstring>
#include <initializer_list>

namespace {
constexpr unsigned kRecord = 23984;
static_assert(sizeof(PlayerRecord) == kRecord, "records are 23984 bytes apart");
}
extern "C" PlayerRecord tempPlayerStruct[3];
extern "C" PlayerRecord player[3];
PlayerRecord tempPlayerStruct[3];
PlayerRecord player[3];

static int callCount, lastIndex;
static unsigned char seenAtCall[kRecord];
extern "C" void convertIndiciesToPointers__Fi(int index) {
    ++callCount;
    lastIndex = index;
    // the conversion must see the copied record already in place
    std::memcpy(seenAtCall, player[index].bytes, kRecord);
}

int main() {
    static unsigned char expectPlayer[3][kRecord];
    for (unsigned r = 0; r < 3; ++r)
        for (unsigned i = 0; i < kRecord; ++i) {
            tempPlayerStruct[r].bytes[i] = static_cast<unsigned char>(i * 31u + r * 17u + 1u);
            player[r].bytes[i] = static_cast<unsigned char>(0xa0u + r);
            expectPlayer[r][i] = player[r].bytes[i];
        }
    for (int n : {1, 0, 2}) {
        callCount = 0;
        std::memcpy(expectPlayer[n], tempPlayerStruct[n].bytes, kRecord);
        MC_ConvertTempPlayerToReal__Fi(n);
        if (callCount != 1 || lastIndex != n) return 1;
        if (std::memcmp(seenAtCall, tempPlayerStruct[n].bytes, kRecord) != 0) return 2;
        for (int r = 0; r < 3; ++r)
            if (std::memcmp(player[r].bytes, expectPlayer[r], kRecord) != 0) return 3 + r;
    }
    std::puts("convert temp player passed");
    return 0;
}
