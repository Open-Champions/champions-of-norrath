#include "set_music_timer.cpp"
#include <cstdio>
int musicTimer = 5;
int main() {
    const int values[] = {0, 1, -1, 0x7fffffff, 12345};
    for (int v : values) {
        gameSetMusicTimer__Fi(v);
        if (musicTimer != v) return 1;
    }
    std::puts("set_music_timer passed");
    return 0;
}
