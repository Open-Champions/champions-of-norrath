#include "pad_async.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

int sMemCardInitialized, sAsyncFd, sAsyncMode;
unsigned char sIconDefinition[16], sListIcon[16];
char sProductTitle[64];
char sProductId[32] = "BASLUS-20470EQONLINE";

static std::vector<std::string> calls;
alignas(64) static unsigned char table[1152];
static unsigned entry_sizes[18];
static char buffer_store[1 << 16];
static int getdir_result, open_result;
static int sync_index;
static unsigned new_size;
static int writes;
static int write_size;
static bool buffer_cleared;

extern "C" void SetTitle__16VIIconDefinitionPCc(void*, const char*) { calls.push_back("title"); }
extern "C" void SetIcon__16VIIconDefinitionPvi(void*, void*, int size) { calls.push_back(size == 0x1cdd8 ? "icon" : "badicon"); }
extern "C" void* __wrap_memalign(unsigned alignment, unsigned size) {
    calls.push_back("memalign" + std::to_string(alignment) + "_" + std::to_string(size));
    return table;
}
extern "C" void __wrap_free(void* p) { calls.push_back(p == table ? "free" : "badfree"); }
extern "C" void* __builtin_vec_new(unsigned size) {
    new_size = size;
    std::memset(buffer_store, 0x55, sizeof buffer_store);
    calls.push_back("new");
    return buffer_store;
}
extern "C" void __builtin_vec_delete(void* p) { calls.push_back(p == buffer_store ? "delete" : "baddelete"); }
extern "C" int sceMcGetDir(int port, int slot, const char* name, unsigned mode, int max, void* t) {
    calls.push_back(std::string("getdir ") + name + (port || slot || mode || max != 18 || t != table ? " bad" : ""));
    for (int i = 0; i < 18; ++i) std::memcpy(table + 16 + 64 * i, &entry_sizes[i], 4);
    return getdir_result;
}
extern "C" int sceMcOpen(int port, int slot, const char* path, int mode) {
    calls.push_back(std::string("open ") + path + (port || slot || mode != 514 ? " bad" : ""));
    return 0;
}
extern "C" int sceMcWrite(int fd, const void* b, int size) {
    ++writes;
    write_size = size;
    buffer_cleared = true;
    for (int i = 0; i < size; ++i) if (static_cast<const char*>(b)[i] != 0) buffer_cleared = false;
    calls.push_back("write" + std::to_string(fd));
    return 0;
}
extern "C" int sceMcSync(int mode, int*, int* result) {
    calls.push_back("sync" + std::to_string(mode));
    if (sync_index++ == 0) return 1;  // result already holds the GetDir return
    *result = open_result;
    return 1;
}

static int run(int init, int total, int entries, int opened) {
    calls.clear();
    sMemCardInitialized = init;
    sAsyncFd = 99;
    sAsyncMode = 0;
    getdir_result = entries;
    open_result = opened;
    sync_index = 0;
    writes = 0;
    return VIMemCardPadAsync__Fi(total);
}

int main() {
    entry_sizes[0] = 1;
    entry_sizes[1] = 1024;
    entry_sizes[2] = 1025;
    entry_sizes[3] = 100000;
    // Negative GetDir result: free, -1, nothing else.
    if (run(1, 50, -2, 0) != -1 || sAsyncMode != 2) return 1;
    if (calls.back() != "free" || calls.size() != 4) return 2;
    // 4 entries: 1 + 1 + 2 + 98 = 102, plus 2 + 2 = 106 used.
    if (run(1, 106 + 3, 4, 5) != 0) return 3;
    if (new_size != 3u << 10 || write_size != 3 << 10 || !buffer_cleared || writes != 1 || sAsyncFd != 5) return 4;
    if (calls[1] != "getdir /BASLUS-20470EQONLINE/*" || calls[4] != "open /BASLUS-20470EQONLINE/pad.dat") return 5;
    if (calls[0] != "memalign64_1152" || calls[calls.size() - 2] != "free" || calls.back() != "delete") return 6;
    // First use runs the title and icon setup.
    if (run(0, 106 + 3, 4, 5) != 0 || calls[1] != "title" || calls[2] != "icon" || sMemCardInitialized != 1) return 7;
    if (run(1, 106 + 1, 4, -3) != -2 || writes != 0 || sAsyncFd != 99) return 8;
    if (calls[calls.size() - 2] != "free" || calls.back() != "delete") return 9;
    if (run(1, 106 + 1, 4, -8) != -1 || writes != 0) return 10;
    // Empty directory: 0 + 2 used.
    if (run(1, 2 + 1, 0, 1) != 0 || write_size != 1024) return 11;
    // Odd counts round the half down.
    if (run(1, 3 + 1, 1, 1) != 0 || write_size != 1024) return 12;
    std::puts("memcard pad-async passed");
    return 0;
}
