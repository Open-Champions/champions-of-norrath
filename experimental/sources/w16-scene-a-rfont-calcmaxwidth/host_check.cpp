// Host check only; not a build input.
#include "calc_max_width.cpp"
#include <cstdio>
int main() {
    alignas(8) unsigned char font[64];
    unsigned char w[6] = {3, 200, 17, 255, 9, 0};
    *(int*)(font + 24) = 77; *(int*)(font + 20) = 5; *(int*)(font + 28) = 6;
    bool ok = CalcMaxWidth__7VIRFontiPUc(font, 3, w) == 0 && *(int*)(font + 24) == 200;
    ok = ok && CalcMaxWidth__7VIRFontiPUc(font, 6, w) == 0 && *(int*)(font + 24) == 255;
    ok = ok && CalcMaxWidth__7VIRFontiPUc(font, 0, w) == 0 && *(int*)(font + 24) == 0;
    *(int*)(font + 24) = 77;
    ok = ok && CalcMaxWidth__7VIRFontiPUc(font, -4, w) == 0 && *(int*)(font + 24) == 0;
    ok = ok && *(int*)(font + 20) == 5 && *(int*)(font + 28) == 6;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
