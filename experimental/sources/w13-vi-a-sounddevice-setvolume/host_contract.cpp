#define NORRATH_HOST_ARENA 1
#include "set_volume.cpp"
#include <string.h>
#include <string>

int norrath_overlay_sIopFunctionData_11edf08[4];
static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return a == 0 ? nullptr : arena + a; }

static std::string tr; static float wave_base, xm_base; static int types[4], tcall, first_ret, next_ret, data_at_set[4];
static unsigned char* pbp;
static float waveMethod(void* self) { tr += "w" + std::to_string((unsigned char*)self - arena) + ";"; return wave_base; }
static float xmMethod(void* self) { tr += "x" + std::to_string((unsigned char*)self - arena) + ";"; return xm_base; }
extern "C" void* norrath_host_function(unsigned a) { return a == 1 ? (void*)waveMethod : (void*)xmMethod; }

extern "C" void* Playback__13VISoundDevicei(void*, int id) { tr += "B" + std::to_string(id) + ";"; return pbp; }
extern "C" int Type__C7VISound(const void*) { tr += "T;"; return types[tcall++]; }
extern "C" void* AsWave__7VISound(void*) { tr += "W;"; return arena + 0x3000; }
extern "C" void* AsXm__7VISound(void*) { tr += "X;"; return arena + 0x3100; }
extern "C" int FirstChannel__13VISoundDeviceUl(void*, unsigned long long m) { tr += "F" + std::to_string(m) + ";"; return first_ret; }
extern "C" int NextChannel__13VISoundDeviceUli(void*, unsigned long long m, int c) { tr += "N" + std::to_string(m) + "," + std::to_string(c) + ";"; return next_ret; }
extern "C" void SetIopFunction__FUii(unsigned f, int n) { tr += "S" + std::to_string(f) + "," + std::to_string(n) + ";"; memcpy(data_at_set, norrath_overlay_sIopFunctionData_11edf08, sizeof data_at_set); }

static unsigned char* dev = arena;             // device at 0
static unsigned char* pb = arena + 0x2000;     // playback
static void fresh() {
    memset(arena, 0, sizeof arena); tr.clear(); tcall = 0; pbp = pb;
    memset(pb, 0x3c, 64);
    *(int*)(pb + 28) = 3; *(int*)(pb + 52) = 1; *(unsigned long long*)(pb + 16) = 0x00000005000000a1ull;
    *(float*)(pb + 44) = 0.25f;
    *(unsigned*)(dev + 2584 + 12) = 0x2800;                      // the sound object
    *(float*)(dev + 3424 + 4) = 2.0f; *(int*)(dev + 3432) = 1;
    // wave at 0x3000: vtable at 0x3400, entry 1 at +16: delta 8, function 1
    *(unsigned*)(arena + 0x3000 + 12) = 0x3400; *(short*)(arena + 0x3400 + 16) = 8; *(unsigned*)(arena + 0x3400 + 20) = 1;
    *(unsigned*)(arena + 0x3100 + 12) = 0x3500; *(short*)(arena + 0x3500 + 16) = -4; *(unsigned*)(arena + 0x3500 + 20) = 2;
    first_ret = 4; next_ret = 9; wave_base = 0.5f; xm_base = 0.5f;
}
int main() {
    fresh(); pbp = 0;
    SetVolume__13VISoundDeviceif(dev, 7, 0.5f);
    if (tr != "B7;") return 1;
    fresh(); *(int*)(pb + 28) = -1;
    SetVolume__13VISoundDeviceif(dev, 7, 3.0f);
    if (tr != "B7;" || *(float*)(pb + 40) != 1.0f) return 2;                 // limited to 1.0, no sound
    fresh(); *(unsigned*)(dev + 2584 + 12) = 0;
    SetVolume__13VISoundDeviceif(dev, 7, -2.0f);
    if (tr != "B7;" || *(float*)(pb + 40) != 0.0f) return 3;
    // wave: 0.5 * 0.5 * 2.0 * 16383 = 8191.5 -> 8191; pan 0.25 * 16383 = 4095.75 -> 4095
    fresh(); types[0] = 0; types[1] = 0;
    SetVolume__13VISoundDeviceif(dev, 7, 0.5f);
    if (tr != "B7;T;W;w12296;F21474836641;N21474836641,4;S6,4;T;") return 4;
    if (data_at_set[0] != 4 || data_at_set[1] != 9 || data_at_set[2] != 8191 || data_at_set[3] != 4095) return 5;
    if (*(float*)(pb + 40) != 0.5f) return 6;
    // flag clear: pan not sent; the pan value of the stored float is not used for data[3]
    fresh(); types[0] = 0; types[1] = 0; *(int*)(dev + 3432) = 0;
    SetVolume__13VISoundDeviceif(dev, 7, 0.5f);
    if (data_at_set[3] != 0 || data_at_set[2] != 8191) return 7;
    // large product takes the >= 2^31 path
    fresh(); types[0] = 0; types[1] = 0; wave_base = 200000.0f; *(float*)(dev + 3424 + 4) = 1.0f;
    SetVolume__13VISoundDeviceif(dev, 7, 1.0f);
    { float f = 200000.0f * 1.0f * 1.0f * 16383.0f; if (data_at_set[2] != (int)(unsigned)f) return 8; if (data_at_set[2] >= 0) return 9; }
    // negative pan truncates toward zero
    fresh(); types[0] = 0; types[1] = 0; *(float*)(pb + 44) = -0.00001f;
    SetVolume__13VISoundDeviceif(dev, 7, 0.5f);
    if (data_at_set[3] != 0) return 10;
    *(float*)(pb + 44) = -1.0f; tr.clear(); tcall = 0;
    SetVolume__13VISoundDeviceif(dev, 7, 0.5f);
    if (data_at_set[3] != -16383) return 11;
    // xm: base 0.5 * volume 1.0 * group 2.0 = 1.0, * 255 = 255; pan 0.25 -> (0.625) * 255 = 159.375 -> 159
    fresh(); types[0] = 1; types[1] = 1;
    SetVolume__13VISoundDeviceif(dev, 7, 1.0f);
    if (tr != "B7;T;T;X;x" + std::to_string(0x3100 - 4) + ";S10,4;") return 12;
    // mask 0x5000000a1: low 24 bits 0xa1; bits 24..55 = 0x500
    if (data_at_set[0] != 0xa1 || data_at_set[1] != 0x500) return 15;
    if (data_at_set[2] != 255 || data_at_set[3] != 159) return 13;
    // other type: both Type calls and nothing else
    fresh(); types[0] = 2; types[1] = 2;
    SetVolume__13VISoundDeviceif(dev, 7, 1.0f);
    if (tr != "B7;T;T;") return 14;
    return 0;
}
