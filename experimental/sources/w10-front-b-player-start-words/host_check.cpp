#include "player_start_words.cpp"
#include <cstdio>

int main() {
    int bad = sizeof(playerStartYaw) != 8 || sizeof(playerSpecStorage) != 4 * 41;
    for (int i = 0; i < 4; ++i) bad |= playerStartYaw[i] != 0;
    for (unsigned i = 0; i < sizeof(playerSpecStorage); ++i) bad |= playerSpecStorage[i] != 0;
    std::printf(bad ? "FAIL\n" : "ok: 8 and 164 zero bytes\n");
    return bad;
}
