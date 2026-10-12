#define NORRATH_HOST_CONTRACT
#include "light_vu0_stopped.cpp"

#include <cassert>
#include <cstdio>
#include <cstring>

extern "C" unsigned char scene[56] = {0};
extern "C" float worldCharViewMatrix[16] = {0};
extern "C" float worldCameraPos[3] = {0};
extern "C" int interfaceMode = 0;
extern "C" int globalCharLightScale = 0;

static int reset_calls;
extern "C" void lightResetOptionalParams__Fv() { ++reset_calls; }

static float vu_stores[9];
static unsigned vu_addresses[9];
static int vu_count;
extern "C" void norrath_store_float(unsigned address, float value) {
    assert(vu_count < 9);
    vu_addresses[vu_count] = address;
    vu_stores[vu_count] = value;
    ++vu_count;
}

static void reset_state() {
    reset_calls = 0;
    vu_count = 0;
    std::memset(scene, 0, sizeof(scene));
    for (int i = 0; i < 16; ++i) worldCharViewMatrix[i] = 0.0f;
    worldCameraPos[0] = 0.0f;
    worldCameraPos[1] = 0.0f;
    worldCameraPos[2] = 4.0f;
    interfaceMode = 0;
    globalCharLightScale = 0;
}

static void check_vu_setup(float biased) {
    assert(vu_count == 9);
    assert(vu_addresses[0] == 0x1100ffccu && vu_stores[0] == 8388736.0f);
    assert(vu_addresses[1] == 0x1100ffc8u && vu_stores[1] == biased);
    assert(vu_addresses[2] == 0x1100ffc0u && vu_stores[2] == biased);
    assert(vu_addresses[3] == 0x1100ffc4u && vu_stores[3] == biased);
    assert(vu_addresses[4] == 0x1100ffdcu && vu_stores[4] == 8388608.0f);
    assert(vu_addresses[5] == 0x1100ffd0u && vu_stores[5] == 8388608.0f);
    assert(vu_addresses[6] == 0x1100ffd4u && vu_stores[6] == 8388608.0f);
    assert(vu_addresses[7] == 0x1100ffd8u && vu_stores[7] == 8388608.0f);
    assert(vu_addresses[8] == 0x1100ffacu && vu_stores[8] == 1.0f);
}

int main() {
    // Identity scale path: interfaceMode zero forces scale 1.0 even when
    // the scale word holds a different value.
    reset_state();
    scene[52] = 0;
    scene[53] = 0;
    scene[54] = 0;
    scene[55] = 0;
    globalCharLightScale = 0x40000000;  // 2.0f, must be ignored here
    for (int i = 0; i < 8; ++i) scene[44 + i] = static_cast<unsigned char>(100 + i);
    for (int i = 0; i < 16; ++i) worldCharViewMatrix[i] = 1.0f;
    lightVU0StoppedProcessing__Fv();
    assert(reset_calls == 2);
    check_vu_setup(-2147483648.0f);  // 0x80000000 wraps to INT32_MIN, exact
    for (int i = 0; i < 8; ++i) assert(scene[36 + i] == 100 + i);

    // Nonzero interfaceMode applies the scale word as a float (0.5 halves).
    reset_state();
    interfaceMode = 7;
    globalCharLightScale = 0x3f000000;  // 0.5f
    scene[44] = 100;
    scene[45] = 101;
    scene[46] = 3;
    scene[47] = 1;
    scene[48] = 0;
    scene[49] = 200;
    scene[50] = 201;
    scene[51] = 255;
    lightVU0StoppedProcessing__Fv();
    assert(reset_calls == 2);
    assert(scene[36] == 50 && scene[37] == 50);  // 100*0.5, 101*0.5 rounds to 50
    assert(scene[38] == 2 && scene[39] == 0);    // 1.5 rounds to even 2, 0.5 to 0
    assert(scene[40] == 0 && scene[41] == 100 && scene[42] == 100);
    assert(scene[43] == 128);  // 127.5 rounds to even 128

    // Clamp edges: above-255 saturation and negative saturation.
    reset_state();
    interfaceMode = 1;
    globalCharLightScale = 0x40000000;  // 2.0f
    scene[44] = 200;  // 400 -> 255
    scene[45] = 128;  // 256 -> 255
    scene[46] = 127;  // 254 passthrough
    scene[47] = 0;
    scene[48] = 1;  // 2 passthrough
    scene[49] = 2;
    scene[50] = 3;
    scene[51] = 4;
    lightVU0StoppedProcessing__Fv();
    assert(scene[36] == 255 && scene[37] == 255 && scene[38] == 254);
    assert(scene[39] == 0 && scene[40] == 2 && scene[41] == 4);
    assert(scene[42] == 6 && scene[43] == 8);

    // Biased word conversion: 0x80000000 + bias wraps to zero.
    reset_state();
    scene[52] = 0;
    scene[53] = 0;
    scene[54] = 0;
    scene[55] = 0x00;  // word 0 -> (int)0x80000000 -> INT32_MIN
    lightVU0StoppedProcessing__Fv();
    check_vu_setup(-2147483648.0f);
    reset_state();
    scene[52] = 0;
    scene[53] = 0;
    scene[54] = 0;
    scene[55] = 0x80;  // word 0x80000000 -> wraps to 0
    lightVU0StoppedProcessing__Fv();
    check_vu_setup(0.0f);

    std::puts("light vu0 stopped contract passed");
    return 0;
}
