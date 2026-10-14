// Host check only; not a build input.
static int calls, lastFunction, lastCount;
extern "C" void SetIopFunction__FUii(unsigned function, int count) { ++calls; lastFunction = function; lastCount = count; }
extern "C" { int norrath_overlay_sIopFunctionData_11edf08[4]; }
#include "stop_all_bgm.cpp"
int main() {
    alignas(8) unsigned char dev[4000];
    for (unsigned char& b : dev) b = 0xaa;
    norrath_overlay_sIopFunctionData_11edf08[0] = 77;
    dev[3948] = 0;
    StopAllBgm__13VISoundDevice(dev);
    if (calls != 0 || dev[3436] != 0 || dev[3692] != 0 || dev[3948] != 0 || norrath_overlay_sIopFunctionData_11edf08[0] != 77) return 1;
    if (dev[3437] != 0xaa || dev[3693] != 0xaa) return 2;
    dev[3948] = 0x80;
    dev[3436] = 1;
    StopAllBgm__13VISoundDevice(dev);
    if (calls != 1 || lastFunction != 14 || lastCount != 1 || dev[3948] != 0 || dev[3436] != 0 || dev[3692] != 0) return 3;
    if (norrath_overlay_sIopFunctionData_11edf08[0] != 0) return 4;
    return 0;
}
