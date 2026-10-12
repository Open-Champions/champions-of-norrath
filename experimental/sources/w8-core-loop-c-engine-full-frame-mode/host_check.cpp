// Host contract for engineFullFrameMode. Not an EE plan input.
#include "engine_full_frame_mode.cpp"

int main() {
    if (engineFullFrameMode != 0) return 1;
    if (sizeof engineFullFrameMode != 4u) return 2;
    engineFullFrameMode = engineFullFrameMode + 1;
    if (engineFullFrameMode != 0 + 1) return 3;
    return 0;
}
