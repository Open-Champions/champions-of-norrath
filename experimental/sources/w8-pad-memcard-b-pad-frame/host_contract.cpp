#include "pad_frame.cpp"

int main() {
    if (sizeof pad_frame != 4u) return 1;
    if (pad_frame != 0u) return 2;
    pad_frame = 7;
    if (pad_frame != 7u) return 3;
    return 0;
}
