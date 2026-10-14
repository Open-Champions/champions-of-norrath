#include "tripple_buffer_mode.cpp"
int main() {
    static_assert(sizeof(trippleBufferMode) == 4, "one word");
    if (trippleBufferMode != 1) return 1;
    trippleBufferMode = 0;
    return trippleBufferMode == 0 ? 0 : 2;
}
