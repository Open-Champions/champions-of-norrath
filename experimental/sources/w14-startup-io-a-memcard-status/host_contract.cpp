#include "status.cpp"
#include <cstring>
#include <string>
#include <vector>

int sMemCardInitialized, sAsyncFd;
unsigned char sIconDefinition[16], sListIcon[16];
char sProductTitle[64];

static std::vector<std::string> calls;
static int polls, info_calls, info_fail_count, set_type, set_free, set_format, sync_zero_polls, sync_return, set_result;
static void* seen_title_target; static void* seen_icon_target; static int seen_icon_size;

extern "C" void SetTitle__16VIIconDefinitionPCc(void* d, const char* t) { calls.push_back("title"); seen_title_target = d; if (t != sProductTitle) calls.push_back("badtitle"); }
extern "C" void SetIcon__16VIIconDefinitionPvi(void* d, void* i, int n) { calls.push_back("icon"); seen_icon_target = d; seen_icon_size = n; if (i != sListIcon) calls.push_back("badicon"); }
extern "C" int sceMcGetInfo(int port, int slot, int* type, int* free, int* format) {
    calls.push_back("info" + std::to_string(port) + std::to_string(slot));
    if (info_calls++ < info_fail_count) return -1;
    *type = set_type; *free = set_free; *format = set_format;
    return 0;
}
extern "C" int sceMcSync(int mode, int* cmd, int* result) {
    calls.push_back("sync" + std::to_string(mode) + (cmd == nullptr ? "n" : "p"));
    if (polls++ < sync_zero_polls) return 0;
    *result = set_result;
    return sync_return;
}
extern "C" void sceGsSyncV(int f) { calls.push_back("vsync" + std::to_string(f)); }

static int expected(int type, int sync, int result, int format) {
    if (type == 1 || sync < 0) return 5;
    if (result < -9) return 1;
    switch (result) {
    case -2: return 2;
    case -5: return 5;
    case -1: return format == 0 ? 4 : 3;
    case 0: return format == 0 ? 2 : 0;
    default: return 0;
    }
}

int main() {
    const int results[] = {-100, -10, -9, -8, -6, -5, -4, -3, -2, -1, 0, 1, 2, 77};
    for (int init = 0; init < 2; ++init)
    for (int type = 0; type < 3; ++type)
    for (int format = 0; format < 3; ++format)
    for (int sync : {1, 2, -1})
    for (int result : results)
    for (int out = 0; out < 2; ++out) {
        calls.clear(); polls = 0; info_calls = 0; info_fail_count = init + 1; set_type = type; set_free = 0x1234 + type;
        set_format = format; sync_zero_polls = type; sync_return = sync; set_result = result;
        sMemCardInitialized = init; sAsyncFd = 55;
        int freeWord = 99;
        int code = VIMemCardStatus__FPi(out ? &freeWord : nullptr);
        if (code != expected(type, sync, result, format)) return 1;
        if (sMemCardInitialized != 1) return 2;
        bool setup = init == 0;
        if (setup != (sAsyncFd == -1)) return 3;
        if (setup != (calls[0] == "title")) return 4;
        if (setup && (calls[1] != "icon" || seen_icon_size != 0x1cdd8 || seen_title_target != sIconDefinition || seen_icon_target != sIconDefinition)) return 5;
        size_t first = setup ? 2 : 0;
        for (int i = 0; i < info_fail_count + 1; ++i) if (calls[first + i] != "info00") return 6;
        size_t sync_at = first + info_fail_count + 1;
        for (int i = 0; i < sync_zero_polls; ++i) {
            if (calls[sync_at + 2 * i] != "sync1n" || calls[sync_at + 2 * i + 1] != "vsync0") return 7;
        }
        if (calls[sync_at + 2 * sync_zero_polls] != "sync1n" || calls.size() != sync_at + 2 * sync_zero_polls + 1) return 8;
        bool wrote = out && !(type == 1 || sync < 0);
        if (freeWord != (wrote ? 0x1234 + type : 99)) return 9;
        for (const std::string& c : calls) if (c.rfind("bad", 0) == 0) return 10;
    }
    return 0;
}
