#include "sequence_state.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof(sequence) == 2432 && sizeof(delaySoundList) == 1152, "record spans");
    static_assert(sizeof(groupVolumeScale) == 1200, "300 words");
    if (sequenceInProgress != -1 || sequenceVolume != 1.0f || soundFrameNm != 0) return 1;
    if (nextChannel != 0 || nmBatchCommands != 0 || disableSoundPlaying != 0) return 2;
    for (float f : groupVolumeScale) if (f != 1.0f) return 3;
    for (unsigned char b : sequence) if (b) return 4;
    for (unsigned char b : delaySoundList) if (b) return 5;
    std::puts("sequence state passed");
    return 0;
}
