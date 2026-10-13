#include <stdint.h>
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<uintptr_t>(word))
#include "set_pitch.cpp"
#include <string.h>
#include <string>

int norrath_overlay_sIopFunctionData_11edf08[4];
static std::string tr; static unsigned char* pbp; static int types[4], tcall, rate, first_ret, next_ret;
static int data_at_set[4];
static std::string h(const void* p) { return std::to_string((uintptr_t)p); }
extern "C" void* Playback__13VISoundDevicei(void*, int id) { tr += "B" + std::to_string(id) + ";"; return pbp; }
extern "C" int Type__C7VISound(const void* s) { tr += "T" + h(s) + ";"; return types[tcall++]; }
extern "C" void* AsWave__7VISound(void* s) { tr += "W;"; return s; }
extern "C" int SampleRate__C6VIWave(const void*) { tr += "R;"; return rate; }
extern "C" void* AsXm__7VISound(void* s) { tr += "X;"; return s; }
extern "C" int FirstChannel__13VISoundDeviceUl(void*, unsigned long long m) { tr += "F" + std::to_string(m) + ";"; return first_ret; }
extern "C" int NextChannel__13VISoundDeviceUli(void*, unsigned long long m, int c) { tr += "N" + std::to_string(m) + "," + std::to_string(c) + ";"; return next_ret; }
extern "C" void SetIopFunction__FUii(unsigned f, int n) { tr += "S" + std::to_string(f) + "," + std::to_string(n) + ";"; memcpy(data_at_set, norrath_overlay_sIopFunctionData_11edf08, sizeof data_at_set); }

int main() {
    alignas(8) static unsigned char dev[3000]; alignas(8) static unsigned char pb[64];
    auto fresh = [&] { memset(dev, 0, sizeof dev); memset(pb, 0x3c, sizeof pb); tr.clear(); tcall = 0; pbp = pb; };
    fresh(); pbp = 0;
    SetPitch__13VISoundDeviceii(dev, 9, 5);
    if (tr != "B9;") return 1;
    // slot -1: pitch stored, no sound
    fresh(); *(int*)(pb + 28) = -1;
    SetPitch__13VISoundDeviceii(dev, 9, 5);
    if (tr != "B9;" || *(int*)(pb + 36) != 5) return 2;
    // empty slot word
    fresh(); *(int*)(pb + 28) = 3;
    SetPitch__13VISoundDeviceii(dev, 9, 6);
    if (tr != "B9;" || *(int*)(pb + 36) != 6) return 3;
    // wave sound: the second Type call returns 0 as well
    fresh(); *(int*)(pb + 28) = 3; *(unsigned*)(dev + 2584 + 12) = 0x777; *(unsigned long long*)(pb + 16) = 0x00000005000000a1ull;
    types[0] = 0; types[1] = 0; rate = 22050; first_ret = 4; next_ret = 9;
    SetPitch__13VISoundDeviceii(dev, 9, 3 << 12);
    if (tr != "B9;T1911;W;R;F21474836641;N21474836641,4;S5,3;T1911;") return 4;
    int v = (22050 << 12) / 48000 * (3 << 12);
    if (data_at_set[0] != 4 || data_at_set[1] != 9 || data_at_set[2] != v / 4096) return 5;
    // negative pitch rounds toward zero
    fresh(); *(int*)(pb + 28) = 3; *(unsigned*)(dev + 2584 + 12) = 0x777;
    types[0] = 0; types[1] = 0; rate = 22050;
    SetPitch__13VISoundDeviceii(dev, 9, -4095);
    v = (22050 << 12) / 48000 * -4095;
    if (data_at_set[2] != v / 4096 || v / 4096 == (v >> 12)) return 6;
    // XM sound: first Type returns 1, second 1
    fresh(); *(int*)(pb + 28) = 3; *(unsigned*)(dev + 2584 + 12) = 0x777; *(unsigned long long*)(pb + 16) = 0xff80000012345678ull;
    types[0] = 1; types[1] = 1;
    SetPitch__13VISoundDeviceii(dev, 9, 77);
    if (tr != "B9;T1911;T1911;X;S11,3;") return 7;
    // bits 24..55 of the mask are 0x80000012 (bit 55 set, so the word is negative); the low 24 bits are 0x345678
    if (data_at_set[0] != 0x345678 || data_at_set[1] != (int)0x80000012u || data_at_set[2] != 77) return 8;
    // other type: both Type calls, nothing else
    fresh(); *(int*)(pb + 28) = 3; *(unsigned*)(dev + 2584 + 12) = 0x777;
    types[0] = 2; types[1] = 2;
    SetPitch__13VISoundDeviceii(dev, 9, 1);
    if (tr != "B9;T1911;T1911;") return 9;
    return 0;
}
