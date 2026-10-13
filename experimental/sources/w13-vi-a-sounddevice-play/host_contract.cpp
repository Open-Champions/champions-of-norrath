#define NORRATH_HOST_ARENA 1
#include "play.cpp"
#include <string.h>
#include <string>

int norrath_overlay_sIopFunctionData_11edf08[8];
static unsigned char arena[0x10000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return a == 0 ? nullptr : arena + a; }
extern "C" unsigned norrath_host_word(const void* p) { return p ? (unsigned)((const unsigned char*)p - arena) : 0; }

static std::string tr; static int types[4], tcall, ready, next_ret, data_at_set[8]; static unsigned long long alloc_ret;
static float f_wpan, f_wbase, f_xpan, f_xbase;
static std::string off(const void* p) { return std::to_string((const unsigned char*)p - arena); }
static int vReady(void* s) { tr += "r" + off(s) + ";"; return ready; }
static float vWavePan(void* s) { tr += "wp" + off(s) + ";"; return f_wpan; }
static float vWaveBase(void* s) { tr += "wb" + off(s) + ";"; return f_wbase; }
static float vXmPan(void* s) { tr += "xp" + off(s) + ";"; return f_xpan; }
static float vXmBase(void* s) { tr += "xb" + off(s) + ";"; return f_xbase; }
extern "C" void* norrath_host_function(unsigned a) {
    switch (a) { case 1: return (void*)vReady; case 2: return (void*)vWavePan; case 3: return (void*)vWaveBase; case 4: return (void*)vXmPan; default: return (void*)vXmBase; }
}
enum { SND = 0x3000, WAVE = 0x3100, XM = 0x3200, VTS = 0x3400, VTW = 0x3500, VTX = 0x3600, PB = 0x4000 };
extern "C" int Type__C7VISound(const void*) { tr += "T;"; return types[tcall++]; }
extern "C" void* AsWave__7VISound(void*) { tr += "W;"; return arena + WAVE; }
extern "C" void* AsXm__7VISound(void*) { tr += "X;"; return arena + XM; }
extern "C" int ChannelCount__C6VIWave(const void*) { tr += "Cw;"; return 2; }
extern "C" int ChannelCount__C4VIXm(const void*) { tr += "Cx;"; return 3; }
extern "C" unsigned long long AllocateChannels__13VISoundDeviceii(void*, int c, int a) { tr += "A" + std::to_string(c) + "," + std::to_string(a) + ";"; return alloc_ret; }
extern "C" void* NewPlayback__13VISoundDevice(void*) { tr += "N;"; return arena + PB; }
extern "C" int FirstChannel__13VISoundDeviceUl(void*, unsigned long long m) { tr += "F" + std::to_string(m) + ";"; return 4; }
extern "C" int NextChannel__13VISoundDeviceUli(void*, unsigned long long m, int c) { tr += "n" + std::to_string(m) + "," + std::to_string(c) + ";"; return next_ret; }
extern "C" void SetIopFunction__FUii(unsigned f, int n) { tr += "S" + std::to_string(f) + "," + std::to_string(n) + ";"; memcpy(data_at_set, norrath_overlay_sIopFunctionData_11edf08, sizeof data_at_set); }

static unsigned char* dev = arena + 0x40;   // device (large enough for the offsets used)
static void fresh() {
    memset(arena, 0, sizeof arena); tr.clear(); tcall = 0; ready = 1; next_ret = 9; alloc_ret = 0x00000005000000a1ull;
    f_wpan = 0.25f; f_wbase = 0.5f; f_xpan = 0.5f; f_xbase = 2.0f;
    memset(arena + PB, 0x5c, 64);
    *(unsigned*)(dev + 2584 + 12) = SND;
    *(unsigned*)(arena + SND + 12) = VTS; *(short*)(arena + VTS + 48) = 4; *(unsigned*)(arena + VTS + 52) = 1;
    *(unsigned*)(arena + WAVE + 12) = VTW; *(short*)(arena + VTW + 16) = 8; *(unsigned*)(arena + VTW + 20) = 3;
    *(short*)(arena + VTW + 32) = -4; *(unsigned*)(arena + VTW + 36) = 2;
    *(int*)(arena + WAVE + 32) = 22050; *(int*)(arena + WAVE + 56) = 0x1001; *(int*)(arena + WAVE + 60) = 77;
    *(unsigned*)(arena + XM + 12) = VTX; *(short*)(arena + VTX + 16) = 12; *(unsigned*)(arena + VTX + 20) = 5;
    *(short*)(arena + VTX + 32) = 0; *(unsigned*)(arena + VTX + 36) = 4;
    *(int*)(arena + XM + 24) = 4242; *(int*)(arena + XM + 52) = 99;
    *(int*)(dev + 3096) = 0x77; *(int*)(dev + 3308) = 5; *(float*)(dev + 3424) = 2.0f; *(float*)(dev + 3428) = 0.25f; *(int*)(dev + 3432) = 1;
}
static int play(int index, int arg2, int cls) { return Play__13VISoundDeviceii12VISOUNDCLASS(dev, index, arg2, cls); }

int main() {
    fresh();
    if (play(3, 0, 2) != -1 || play(3, 0, 7) != -1 || play(-1, 0, 0) != -1 || !tr.empty()) return 1;
    *(unsigned*)(dev + 2584 + 12) = 0;
    if (play(3, 0, 0) != -1 || !tr.empty()) return 2;
    fresh(); ready = 0;
    if (play(3, 0, 0) != -2 || tr != "r" + off(arena + SND + 4) + ";") return 3;
    fresh(); alloc_ret = 0; types[0] = 0;
    if (play(3, 9, 0) != -1 || tr != "r" + off(arena + SND + 4) + ";T;W;Cw;A2,9;") return 4;
    fresh(); alloc_ret = 0; types[0] = 5;
    if (play(3, 9, 0) != -1 || tr != "r" + off(arena + SND + 4) + ";T;X;Cx;A3,9;") return 5;
    // all 46 slots busy
    fresh(); types[0] = 0;
    for (int i = 0; i < 46; ++i) *(unsigned*)(dev + 3100 + 4 * i) = 0x100 + i;
    if (play(3, 9, 0) != -1 || tr.find("N;") != std::string::npos) return 6;
    // wave
    fresh(); types[0] = 0; types[1] = 0;
    for (int i = 0; i < 3; ++i) *(unsigned*)(dev + 3100 + 4 * i) = 0x100 + i;
    int id = play(3, 9, 1);
    if (id != (3 | (5 << 8))) return 7;
    if (*(int*)(dev + 3308) != 6 || *(unsigned*)(dev + 3100 + 12) != PB) return 8;
    unsigned char* pb = arena + PB;
    if (*(int*)(pb + 24) != 9 || *(int*)(pb + 28) != 3 || *(int*)(pb + 32) != id || *(unsigned long long*)(pb + 16) != 0x00000005000000a1ull) return 9;
    if (*(int*)(pb + 52) != 1 || *(int*)(pb + 48) != 0x77 || *(int*)(pb + 36) != 4096 || *(float*)(pb + 40) != 1.0f || *(float*)(pb + 44) != 0.25f) return 10;
    // data: first = 4, wave[60] = 77, next = 9, 77 + (0x1001 >> 1), 22050 * 4096 / 48000, 1.0 * 16383 * (0.25 * 0.5), pan 0.25 * 16383
    int rate = (int)((22050LL << 12) / 48000);
    float vol = 1.0f * 16383.0f * (0.25f * 0.5f);
    if (data_at_set[0] != 4 || data_at_set[1] != 77 || data_at_set[2] != 9 || data_at_set[3] != 77 + (0x1001 >> 1) || data_at_set[4] != rate) return 11;
    if (data_at_set[5] != (int)vol || data_at_set[6] != 4095) return 12;
    if (tr != "r" + off(arena + SND + 4) + ";T;W;Cw;A2,9;N;T;W;F21474836641;n21474836641,4;wp" + off(arena + WAVE - 4) + ";wb" + off(arena + WAVE + 8) + ";S2,7;") return 13;
    // wave with no further channel: data[3] = 0; flag clear: no pan value
    fresh(); types[0] = 0; types[1] = 0; next_ret = -1; *(int*)(dev + 3432) = 0;
    play(3, 9, 0);
    if (data_at_set[3] != 0 || data_at_set[2] != -1 || data_at_set[6] != 0) return 14;
    // class 0 uses the first group volume (2.0), a big product takes the >= 2^31 conversion
    fresh(); types[0] = 0; types[1] = 0; f_wbase = 100000.0f;
    play(3, 9, 0);
    { float v = 1.0f * 16383.0f * (2.0f * 100000.0f); if (data_at_set[5] != (int)(unsigned)v || data_at_set[5] >= 0) return 15; }
    // xm
    fresh(); types[0] = 5; types[1] = 1; types[2] = 1;
    id = play(3, 9, 0);
    if (id != (0 | (5 << 8))) return 16;     // no busy slot: slot 0
    if (tr != "r" + off(arena + SND + 4) + ";T;X;Cx;A3,9;N;T;T;X;xp" + off(arena + XM) + ";xb" + off(arena + XM + 12) + ";S8,7;") return 17;
    if (data_at_set[0] != 0xa1 || data_at_set[1] != 0x500 || data_at_set[2] != 4242 || data_at_set[3] != 99 || data_at_set[4] != 4096) return 18;
    // volume 1.0 * 255 * (2.0 * 2.0) = 1020; pan (0.5 * 0.5 + 0.5) * 255 = 191.25 -> 191
    if (data_at_set[5] != 1020 || data_at_set[6] != 191 || *(float*)(pb + 44) != 0.5f || *(float*)(pb + 40) != 1.0f) return 19;
    // third type value other than 1: -1 after the slot was claimed
    fresh(); types[0] = 5; types[1] = 2; types[2] = 2;
    if (play(3, 9, 0) != -1 || *(unsigned*)(dev + 3100) != PB) return 20;
    return 0;
}
