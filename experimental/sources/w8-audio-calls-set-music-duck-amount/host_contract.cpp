#include "set_music_duck_amount.cpp"
#include <cstdio>
float desiredMusicDuckAmount = 9.0f;
int main() {
    soundSetMusicDuckAmount__Ff(0.25f);
    if (desiredMusicDuckAmount != 0.75f) return 1;
    soundSetMusicDuckAmount__Ff(0.0f);
    if (desiredMusicDuckAmount != 1.0f) return 2;
    soundSetMusicDuckAmount__Ff(1.5f);
    if (desiredMusicDuckAmount != -0.5f) return 3;
    std::puts("set_music_duck_amount passed");
    return 0;
}
