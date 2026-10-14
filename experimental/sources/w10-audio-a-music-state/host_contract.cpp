#include "music_state.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof(cutStartMusic) == 48, "gap to the next symbol");
    static_assert(sizeof(norrath_music_activity_ring_view) == 32, "ring size");
    if (musicPlaying != 0 || musicStart != 0 || musicTimer != 14400 || lastMusicState != -1) return 1;
    if (musicDuckAmount != 1.0f || desiredMusicDuckAmount != 0.0f) return 2;
    if (displayMusicPos != 0 || playedStinger != 0 || cutLoadedDialogStream != 0) return 3;
    for (char c : cutStartMusic) if (c) return 4;
    if (norrath_music_activity_ring_view[0] != 5 || norrath_music_activity_ring_view[3] != 26 ||
        norrath_music_activity_ring_view[31] != -57) return 5;
    std::puts("music state passed");
    return 0;
}
