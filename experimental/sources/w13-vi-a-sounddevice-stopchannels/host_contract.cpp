#include "stop_channels.cpp"

int norrath_overlay_sIopFunctionData_11edf08[4];
static int pend[2]; static int* find_ret; static unsigned find_arg; static int set_calls, set_fn, set_n, find_calls;
static int snap0, snap1;
extern "C" int* FindIopFunction__FUi(unsigned f) { ++find_calls; find_arg = f; return find_ret; }
extern "C" void SetIopFunction__FUii(unsigned f, int n) { ++set_calls; set_fn = f; set_n = n; snap0 = norrath_overlay_sIopFunctionData_11edf08[0]; snap1 = norrath_overlay_sIopFunctionData_11edf08[1]; }

int main() {
    // staged path (no pending entry): lo = bits 0..23, hi = bits 24..55 sign-extended from bit 55
    find_ret = 0;
    StopChannels__13VISoundDeviceUl(0, 0xff80000000a5a5a5ull);
    if (find_arg != 4 || set_calls != 1 || set_fn != 4 || set_n != 2) return 1;
    if (snap0 != 0xa5a5a5) return 2;
    if (snap1 != (int)0x80000000u) return 3;           // only bit 55 set among bits 24..55
    StopChannels__13VISoundDeviceUl(0, 0x007fffffffffffffull);
    if (snap0 != 0xffffff || snap1 != 0x7fffffff) return 4;
    StopChannels__13VISoundDeviceUl(0, 0x0000000012345678ull << 24 | 0x00abcdefull);
    if (snap0 != 0xabcdef || snap1 != 0x12345678) return 5;
    // pending path: OR into the existing entry, nothing staged or queued
    pend[0] = 0x1000000; pend[1] = 0x1; find_ret = pend;
    int before = set_calls;
    StopChannels__13VISoundDeviceUl(0, 0x0000000200000f0full << 0);
    if (set_calls != before) return 6;
    if (pend[0] != (0x1000000 | 0x000f0f)) return 7;
    if (pend[1] != (1 | 0x200)) return 8;
    return 0;
}
