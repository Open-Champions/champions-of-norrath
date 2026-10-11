#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" void fxUpdateDust__Fiiiiiiii(int, int, int, int, int, int, int, int);

alignas(8) unsigned char fxDust[80];

struct DustCall {
    void* definition;
    int values[8];
    float amount;
    float rise;
    float sink;
    int mode;
    int unused;
    int repeat;
    void (*callback)(void*);
    int calls;
};

static DustCall dust_call;

extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(
    void* definition, int first, int second, int third, int fourth, int fifth,
    int sixth, int seventh, int eighth, float amount, float rise, float sink,
    int mode, int unused, int repeat, void (*callback)(void*)) {
    dust_call.definition = definition;
    dust_call.values[0] = first;
    dust_call.values[1] = second;
    dust_call.values[2] = third;
    dust_call.values[3] = fourth;
    dust_call.values[4] = fifth;
    dust_call.values[5] = sixth;
    dust_call.values[6] = seventh;
    dust_call.values[7] = eighth;
    dust_call.amount = amount;
    dust_call.rise = rise;
    dust_call.sink = sink;
    dust_call.mode = mode;
    dust_call.unused = unused;
    dust_call.repeat = repeat;
    dust_call.callback = callback;
    dust_call.calls += 1;
}

static unsigned float_bits(float value) {
    unsigned bits = 0;
    std::memcpy(&bits, &value, 4);
    return bits;
}

int main() {
    std::memset(fxDust, 0xab, sizeof fxDust);
    fxUpdateDust__Fiiiiiiii(1, 2, 3, 4, 5, 6, 7, 8);
    if (dust_call.calls != 1) return 1;
    if (dust_call.definition != fxDust) return 2;
    for (int i = 0; i < 8; ++i) {
        if (dust_call.values[i] != i + 1) return 3;
    }
    if (float_bits(dust_call.amount) != 0x41a4ccccu) return 4;
    if (float_bits(dust_call.rise) != 0x41180000u) return 5;
    if (float_bits(dust_call.sink) != 0xc0a00000u) return 6;
    if (dust_call.mode != 2 || dust_call.unused != 0 || dust_call.repeat != 2) return 7;
    if (dust_call.callback != nullptr) return 8;
    fxUpdateDust__Fiiiiiiii(-1, 0, 9, 8, 7, 6, 5, 4);
    if (dust_call.calls != 2) return 9;
    if (dust_call.values[0] != -1 || dust_call.values[7] != 4) return 10;
    std::printf("update dust passed\n");
    return 0;
}
