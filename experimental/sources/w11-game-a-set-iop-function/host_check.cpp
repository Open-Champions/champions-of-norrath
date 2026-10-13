// Host contract for SetIopFunction; not an EE input.
#include "set_iop_function.cpp"
#include <cstdio>
#include <cstring>
int norrath_overlay_sSend_11edb00[128];
int norrath_overlay_sFunctionIndex_11edf00;
int norrath_overlay_sIopFunctionData_11edf08[128];
int main() {
    int bad = 0;
    for (int i = 0; i < 128; ++i) norrath_overlay_sIopFunctionData_11edf08[i] = 1000 + i;
    SetIopFunction__FUii(3, 0);
    if (norrath_overlay_sFunctionIndex_11edf00 != 2 || norrath_overlay_sSend_11edb00[0] != 1 || norrath_overlay_sSend_11edb00[1] != 3 || norrath_overlay_sSend_11edb00[2] != 0) { std::puts("fail: first"); bad = 1; }
    SetIopFunction__FUii(7, 3);
    int* s = norrath_overlay_sSend_11edb00;
    if (norrath_overlay_sFunctionIndex_11edf00 != 7 || s[0] != 2 || s[3] != 7 || s[4] != 3 || s[5] != 1000 || s[6] != 1001 || s[7] != 1002 || s[8] != 0) { std::puts("fail: second"); bad = 1; }
    // index + count + 3 == 128 is rejected, one below is accepted
    norrath_overlay_sFunctionIndex_11edf00 = 120;
    int before[128]; std::memcpy(before, s, sizeof before);
    SetIopFunction__FUii(9, 5);
    if (std::memcmp(before, s, sizeof before) != 0 || norrath_overlay_sFunctionIndex_11edf00 != 120) { std::puts("fail: full"); bad = 1; }
    SetIopFunction__FUii(9, 4);
    if (norrath_overlay_sFunctionIndex_11edf00 != 126 || s[121] != 9 || s[122] != 4 || s[126] != 1003 || s[0] != 3) { std::puts("fail: last fit"); bad = 1; }
    if (!bad) std::puts("SetIopFunction ok");
    return bad;
}
