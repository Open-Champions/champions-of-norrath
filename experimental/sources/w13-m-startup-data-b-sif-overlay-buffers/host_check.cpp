// Host check for sif_overlay_buffers.cpp. Built with the host compiler only.
// It fails if an observed span changes size or if an overlay spelling stops
// naming the same storage as its plain name.
#include <cstdio>
#include "sif_overlay_buffers.cpp"

static_assert(sizeof(sSend) == 512, "sSend is 128 words");
static_assert(sizeof(sRecv) == 512, "sRecv is 128 words");
static_assert(sizeof(sFunctionIndex) == 8, "the index slot is 8 bytes");
static_assert(sizeof(sIopFunctionData) == 512, "argument array is 128 words");
static_assert(sizeof(sSifClient) == 56, "observed span of sSifClient");

int main() {
    int bad = 0;
#if !defined(__APPLE__)
    // Darwin rejects the alias attribute, so these checks run only where it exists.
    if (static_cast<void*>(norrath_overlay_sSend_11edb00) != static_cast<void*>(sSend)) { std::puts("fail: sSend alias"); bad = 1; }
    if (static_cast<void*>(norrath_overlay_sIopFunctionData_11edf08) != static_cast<void*>(sIopFunctionData)) { std::puts("fail: sIopFunctionData alias"); bad = 1; }
#endif
    for (int i = 0; i < 128; ++i) if (sSend[i] != 0 || sRecv[i] != 0 || sIopFunctionData[i] != 0) { std::puts("fail: zero start"); bad = 1; break; }
    if (sFunctionIndex[0] != 0 || sSifClient[55] != 0) { std::puts("fail: zero start slot"); bad = 1; }
    sSend[127] = 7;
    std::puts(bad ? "FAIL" : "PASS");
    return bad;
}
