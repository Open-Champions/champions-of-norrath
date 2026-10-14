#include "next_frame_drop.cpp"
int main() {
    if (sizeof(nextFrameDrop) != 1) return 1;
    if (nextFrameDrop != 0) return 2;
    nextFrameDrop = 1;
    if (nextFrameDrop != 1) return 3;
    return 0;
}
