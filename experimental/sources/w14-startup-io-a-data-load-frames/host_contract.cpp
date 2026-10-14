#include "load_frames.cpp"
int main() {
    static_assert(sizeof(loadFrames) == 4, "four-byte word");
    if (loadFrames != 0) return 1;
    loadFrames = 7;
    return loadFrames == 7 ? 0 : 2;
}
