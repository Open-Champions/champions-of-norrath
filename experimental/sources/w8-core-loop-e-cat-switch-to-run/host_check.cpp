// Host check for switchToRun__3Cat. Not an EE plan input.
#include "switch_to_run.cpp"
#include <cstring>
#include <string>

static std::string trace;
static unsigned char* g_object;
static unsigned char g_byte_at_add[4];
static int adds = 0;

extern "C" void animResetFramestates__FP14AnimationState(AnimationState* state) {
    trace += "R";
    if ((unsigned char*)state != g_object + 288) trace += "?";
    // The byte at +292 and the saved block must be untouched by the reset itself.
    g_object[424] = 0xee;  // clobber the destination so the restore is observable
}
extern "C" void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(
    AnimationState* state, AnimationHeader* header, float a, float b, float c, float d, float e, int flags) {
    trace += "A" + std::to_string((long)(((uintptr_t)header) & 0xff)) + ":" + std::to_string((int)a) + std::to_string((int)b)
        + std::to_string((int)c) + std::to_string((int)d) + std::to_string((int)e) + ":" + std::to_string(flags) + ";";
    if ((unsigned char*)state != g_object + 288) trace += "?";
    if (adds < 4) g_byte_at_add[adds] = g_object[292];
    ++adds;
}

int main() {
    alignas(16) static unsigned char object[512];
    alignas(16) static unsigned char table[128];
    g_object = object;
    for (int i = 0; i < 512; ++i) object[i] = (unsigned char)(i * 7 + 1);
    // Table entries are pointers whose low byte identifies the header.
    *(unsigned char**)(object + 36) = table;
    *(uintptr_t*)(table + 116) = 0x1100;
    *(uintptr_t*)(table + 92) = 0x2200 + 0x21;
    *(uintptr_t*)(table + 100) = 0x3300 + 0x32;
    *(uintptr_t*)(table + 108) = 0x4400 + 0x43;
    object[292] = 99;
    unsigned char block[32];
    std::memcpy(block, object + 296, 32);
    switchToRun__3Cat(object);
    if (trace != "RA0:01010:513;A33:00010:513;A50:00010:513;A67:00010:513;") return 1;
    for (int i = 0; i < 4; ++i) if (g_byte_at_add[i] != 0) return 2;
    if (object[292] != 5) return 3;
    unsigned char expect[32];
    std::memcpy(expect, block, 32);
    float one = 1.0f, thirtieth; unsigned bits = 0xbd088888u; std::memcpy(&thirtieth, &bits, 4);
    std::memcpy(expect + 8, &one, 4);
    std::memcpy(expect + 12, &thirtieth, 4);
    std::memcpy(expect + 16, &one, 4);
    if (std::memcmp(object + 424, expect, 32) != 0) return 5;
    // The source block itself is unchanged.
    if (std::memcmp(object + 296, block, 32) != 0) return 6;
    return 0;
}
