#include "write_file.cpp"

#include <cstdio>
#include <string>
#include <vector>

int sMemCardInitialized, sAsyncFd;
unsigned char sIconDefinition[16], sListIcon[16];
char sProductTitle[64];

static std::vector<std::string> calls;
static int async_return, first_result, second_result;
static unsigned sync_calls;
static int async_size;
static const void* async_buffer;

extern "C" void SetTitle__16VIIconDefinitionPCc(void*, const char*) { calls.push_back("title"); }
extern "C" void SetIcon__16VIIconDefinitionPvi(void*, void*, int size) {
    calls.push_back(size == 0x1cdd8 ? "icon" : "badicon");
}
extern "C" int VIMemCardWriteFileAsync__FPCcPCvi(const char*, const void* buffer, int size) {
    calls.push_back("async");
    async_buffer = buffer;
    async_size = size;
    return async_return;
}
extern "C" int sceMcFlush(int fd) { calls.push_back("flush" + std::to_string(fd)); return 0; }
extern "C" int sceMcClose(int fd) { calls.push_back("close" + std::to_string(fd)); return 0; }
extern "C" int sceMcSync(int mode, int*, int* result) {
    calls.push_back("sync" + std::to_string(mode));
    *result = sync_calls++ == 0 ? first_result : second_result;
    return 1;
}

static int run(int init, int started, int r1, int r2) {
    calls.clear();
    sMemCardInitialized = init;
    sAsyncFd = 11;
    async_return = started;
    first_result = r1;
    second_result = r2;
    sync_calls = 0;
    static const char data[4] = {1, 2, 3, 4};
    int got = VIMemCardWriteFile__FPCcPCvi("save", data, 4);
    if (async_buffer != data || async_size != 4) return 1000;
    return got;
}

int main() {
    if (run(1, -7, 0, 0) != -7 || calls.size() != 1 || calls[0] != "async") return 1;
    // First use runs the title and icon setup before the async call.
    if (run(0, -1, 0, 0) != -1 || calls.size() != 3 || calls[0] != "title" || calls[1] != "icon" || calls[2] != "async") return 2;
    if (sMemCardInitialized != 1 || sAsyncFd != -1) return 3;
    if (run(1, 0, -3, 0) != -2 || calls.size() != 3 || calls[1] != "flush11" || calls[2] != "sync0") return 4;
    if (run(1, 0, -9, 0) != -1 || calls.size() != 3) return 5;
    if (run(1, 5, 5, 0) != 0 || calls.size() != 5 || calls[3] != "close11" || calls[4] != "sync0") return 6;
    if (run(1, 0, 2, -4) != -1 || calls.size() != 5) return 7;
    std::puts("memcard write-file passed");
    return 0;
}
