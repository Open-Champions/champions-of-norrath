// Host contract for engineGetAIStepCount. Not an EE plan input.
#include "engine_get_ai_step_count.cpp"

extern "C" {
int trippleBufferMode = 0;
signed char nextFrameDrop = 0;
int lastFrameVBlankCount = 0;
}

int main() {
    int expect[] = {-5, 1, 0, 1, 1, 1, 2, 2, 3, 2, 99, 2};
    trippleBufferMode = 0;
    nextFrameDrop = 77;  // ignored on this path
    for (int i = 0; i < 12; i += 2) {
        lastFrameVBlankCount = expect[i];
        if (engineGetAIStepCount__Fv() != expect[i + 1]) return 1 + i;
    }
    trippleBufferMode = 5;
    lastFrameVBlankCount = 2;  // ignored on this path
    int drops[] = {-128, 1, -2, 1, -1, 1, 0, 1, 1, 2, 2, 2, 100, 2};
    for (int i = 0; i < 14; i += 2) {
        nextFrameDrop = static_cast<signed char>(drops[i]);
        if (engineGetAIStepCount__Fv() != drops[i + 1]) return 20 + i;
    }
    return 0;
}
