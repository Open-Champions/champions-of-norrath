// Host check only; not a build input.
#include "calc_data_size.cpp"
#include <cstdio>
int main() {
    int f[32];
    unsigned char d[4] = {5, 6, 0, 255};
    const int steps[7][4] = {{3, 3, 0, 128}, {5, 6, 0, 255}, {20, 24, 0, 1020}, {20, 24, 0, 1020}, {15, 18, 0, 765}, {10, 12, 0, 510}, {20, 24, 0, 1020}};
    bool ok = true;
    for (int mode = 0; mode < 7; ++mode) {
        for (int i = 0; i < 32; ++i) f[i] = 1000 + i;
        f[10] = 4; f[7] = mode; f[5] = 3; f[12] = 2; f[13] = 0;
        ok = ok && CalcDataSize__7VIRFontPUc(reinterpret_cast<unsigned char*>(f), d) == 0;
        int sum = 0; for (int i = 0; i < 4; ++i) sum += steps[mode][i] * 3;
        ok = ok && f[16] == 0 && f[17] == 48 && f[19] == 48 + 112 && f[18] == 160 + sum && f[20] == f[18];
    }
    for (int i = 0; i < 32; ++i) f[i] = 1000 + i;
    f[10] = 4; f[7] = 7; f[5] = 3; f[12] = 2; f[13] = 0;
    ok = ok && CalcDataSize__7VIRFontPUc(reinterpret_cast<unsigned char*>(f), d) == -1 && f[18] == 1018 && f[20] == 160;
    f[7] = 1; f[13] = 5; f[8] = 2;  // extra tail: +4 per extra unit
    ok = ok && CalcDataSize__7VIRFontPUc(reinterpret_cast<unsigned char*>(f), d) == 0 && f[20] == f[18] + 20;
    f[8] = 3;
    ok = ok && CalcDataSize__7VIRFontPUc(reinterpret_cast<unsigned char*>(f), d) == -1;
    f[10] = 0; f[13] = 0;
    ok = ok && CalcDataSize__7VIRFontPUc(reinterpret_cast<unsigned char*>(f), d) == 0 && f[20] == f[19] && f[18] == f[19];
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
