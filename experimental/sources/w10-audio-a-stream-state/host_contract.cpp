#include "stream_state.cpp"
#include <cstdio>
int main() {
    static_assert(sizeof(stream) == 1356, "three 452-byte records");
    static_assert(sizeof(streamVolume) == 12, "three volumes");
    for (float f : streamVolume) if (f != 1.0f) return 1;
    if (streaming != 0 || streamOutput != nullptr || streamSizeLeft != 0 || streamPaused != 0) return 2;
    for (unsigned char b : stream) if (b) return 3;
    std::puts("stream state passed");
    return 0;
}
