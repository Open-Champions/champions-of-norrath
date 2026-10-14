#define NORRATH_HOST_CONTRACT 1
#include "sce_gs_sync_v.cpp"

#include <cstdio>
#include <cstring>
#include <string>

static unsigned char param[16];
static int vsync_mode;
static int vsync2_mode;
static int vsync_calls;
static int vsync2_calls;
static int vsync2_result;
static unsigned long long csr;
static std::string log_text;

extern "C" unsigned char* sceGsGetGParam() { return param; }
extern "C" int VSync(int mode) {
    vsync_calls += 1;
    vsync_mode = mode;
    log_text += "V";
    return 0;
}
extern "C" int VSync2(int mode) {
    vsync2_calls += 1;
    vsync2_mode = mode;
    log_text += "2";
    return vsync2_result;
}
extern "C" unsigned long long norrath_host_gs_load64(unsigned address) {
    log_text += address == 0x12001000u ? "C" : "?";
    return csr;
}

static void reset(short interlace, unsigned handler) {
    std::memset(param, 0, sizeof param);
    *reinterpret_cast<short*>(param) = interlace;
    *reinterpret_cast<unsigned*>(param + 8) = handler;
    vsync_calls = 0;
    vsync2_calls = 0;
    log_text.clear();
}

int main() {
    reset(1, 0u);
    csr = 1ull << 13;
    if (sceGsSyncV(0) != 1 || log_text != "VC" || vsync_mode != 0) return 1;
    csr = 0ull;
    if (sceGsSyncV(4) != 0 || vsync_mode != 4) return 2;
    reset(0, 0u);
    if (sceGsSyncV(3) != 1 || log_text != "V" || vsync2_calls != 0) return 3;
    reset(2, 0u);
    if (sceGsSyncV(-1) != 1 || log_text != "V") return 4;
    reset(1, 9u);
    vsync2_result = 1 << 13;
    if (sceGsSyncV(0) != 1 || log_text != "2" || vsync2_mode != 0 || vsync_calls != 0) return 5;
    vsync2_result = 0;
    if (sceGsSyncV(6) != 0 || vsync2_mode != 6) return 6;
    reset(0, 9u);
    vsync2_result = 1 << 13;
    if (sceGsSyncV(1) != 1 || log_text != "2") return 7;
    std::puts("gs sync v contract passed");
    return 0;
}
