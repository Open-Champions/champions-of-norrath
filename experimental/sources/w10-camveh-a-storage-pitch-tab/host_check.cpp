// Host contract for pitchTab storage. Not an EE plan input.
#include "storage.cpp"

int main() {
    if (sizeof(pitchTab) != 512) return 1;
    for (float f : pitchTab) if (f != 0.0f) return 2;
    return 0;
}
