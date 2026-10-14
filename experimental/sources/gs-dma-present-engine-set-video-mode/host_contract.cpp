#include "engine_set_video_mode.cpp"

#include <cstdio>

extern "C" int engineDisable30fps = 0;
extern "C" int compositeAlpha = 0;
extern "C" int fullFrameMode = 0;
extern "C" int engineFullFrameMode = 0;
extern "C" int trippleBufferMode = 0;

static void reset(int disable, int alpha, int frame) {
    engineDisable30fps = disable;
    compositeAlpha = alpha;
    fullFrameMode = frame;
    engineFullFrameMode = 40;
    trippleBufferMode = 41;
}

int main() {
    reset(1, 200, 0);
    engineSetVideoMode__Fb(1);
    if (engineFullFrameMode != 40 || fullFrameMode != 0 || trippleBufferMode != 41) return 1;
    reset(0, 127, 0);
    engineSetVideoMode__Fb(1);
    if (engineFullFrameMode != 40 || trippleBufferMode != 41) return 2;
    reset(0, -1, 0);
    engineSetVideoMode__Fb(1);
    if (fullFrameMode != 0 || engineFullFrameMode != 40) return 3;
    reset(0, 128, 1);
    engineSetVideoMode__Fb(1);
    if (engineFullFrameMode != 40 || fullFrameMode != 1 || trippleBufferMode != 41) return 4;
    reset(0, 128, 0);
    engineSetVideoMode__Fb(1);
    if (engineFullFrameMode != 1 || fullFrameMode != 1 || trippleBufferMode != 0) return 5;
    reset(0, 200, 1);
    engineSetVideoMode__Fb(0);
    if (engineFullFrameMode != 0 || fullFrameMode != 0 || trippleBufferMode != 1) return 6;
    reset(0, 128, 4);
    engineSetVideoMode__Fb(5);
    if (engineFullFrameMode != 5 || fullFrameMode != 5 || trippleBufferMode != 4) return 7;
    std::puts("engine set video mode contract passed");
    return 0;
}
