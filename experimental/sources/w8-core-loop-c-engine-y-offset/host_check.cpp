// Host contract for engineYOffset. Not an EE plan input.
#include "engine_y_offset.cpp"

int main() {
    if (engineYOffset != 0) return 1;
    if (sizeof engineYOffset != 4u) return 2;
    engineYOffset = engineYOffset + 1;
    if (engineYOffset != 0 + 1) return 3;
    return 0;
}
