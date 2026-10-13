#include "read_file.cpp"
#include <string>
#include <vector>

int sMemCardInitialized, sAsyncFd;
unsigned char sIconDefinition[16], sListIcon[16];
char sProductTitle[64];

static std::vector<std::string> calls;
static int async_result, sync_results[2], sync_index, sync_writes;
static const char* seen_name; static void* seen_buffer; static int seen_size;
static int seen_close_fd;

extern "C" void SetTitle__16VIIconDefinitionPCc(void*, const char*) { calls.push_back("title"); }
extern "C" void SetIcon__16VIIconDefinitionPvi(void*, void*, int n) { calls.push_back("icon" + std::to_string(n)); }
extern "C" int VIMemCardReadFileAsync__FPCcPvi(const char* n, void* b, int s) {
    calls.push_back("async"); seen_name = n; seen_buffer = b; seen_size = s; return async_result;
}
extern "C" int sceMcSync(int mode, int* cmd, int* result) {
    calls.push_back("sync" + std::to_string(mode));
    if (cmd == nullptr || result == nullptr) calls.push_back("badptr");
    if (sync_writes) *result = sync_results[sync_index];
    ++sync_index;
    return 1;
}
extern "C" int sceMcClose(int fd) { calls.push_back("close"); seen_close_fd = fd; return 0; }

int main() {
    const int firsts[] = {-9, -4, -3, -2, -1, 0, 1, 4096};
    const int seconds[] = {-1, -3, 0, 5};
    char name[] = "x";
    char buffer[4];
    for (int init = 0; init < 2; ++init)
    for (int fd : {-7, -1, 0, 5, 30})
    for (int first : firsts)
    for (int second : seconds)
    for (int writes = 0; writes < 2; ++writes) {
        calls.clear(); sync_index = 0; sync_writes = writes;
        async_result = fd; sync_results[0] = first; sync_results[1] = second;
        sMemCardInitialized = init; sAsyncFd = 1234;
        int code = VIMemCardReadFile__FPCcPvi(name, buffer, 17);
        if (seen_name != name || seen_buffer != buffer || seen_size != 17) return 1;
        if (sMemCardInitialized != 1) return 2;
        if ((init == 0) != (sAsyncFd == -1)) return 3;
        if ((init == 0) != (calls[0] == "title") || (init == 0 && calls[1] != "icon" + std::to_string(0x1cdd8))) return 4;
        size_t base = init == 0 ? 2 : 0;
        if (calls[base] != "async") return 5;
        int got = writes ? first : fd;       // an unwritten result keeps the async value
        int closeStatus = writes ? second : fd;
        if (fd < 0) {
            if (code != fd || calls.size() != base + 1) return 6;
            continue;
        }
        if (calls[base + 1] != "sync0") return 7;
        int want;
        size_t steps;
        if (got == -3) { want = -2; steps = 2; }
        else if (got < 0) { want = -1; steps = 2; }
        else {
            if (calls[base + 2] != "close" || seen_close_fd != sAsyncFd || calls[base + 3] != "sync0") return 8;
            want = closeStatus < 0 ? -1 : got;
            steps = 4;
        }
        if (code != want || calls.size() != base + steps) return 9;
        for (const std::string& c : calls) if (c == "badptr") return 10;
    }
    return 0;
}
