// Host contract for initLump__FPv. Not an EE plan input.
#define NORRATH_HOST_CONTRACT 1
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <vector>
#include <string>
#include "init_lump.cpp"

static const unsigned kVirtualBase = 0x40000000u;
alignas(16) static unsigned char lumpMemory[0x2000];
static int nullStores;
static int sink;
unsigned norrath_host_address(const void* p) {
    return kVirtualBase + static_cast<unsigned>(static_cast<const unsigned char*>(p) - lumpMemory);
}
void* norrath_host_pointer(unsigned a) {
    if (a == 0) { ++nullStores; return &sink; }
    return lumpMemory + (a - kVirtualBase);
}
static std::vector<std::string> events;
static std::vector<int> initFileResults;
static unsigned initFileIndex;
extern "C" {
int scePrintf(const char* f, ...) {
    va_list ap; va_start(ap, f);
    const char* s = va_arg(ap, const char*);
    va_end(ap);
    events.push_back(std::string("printf:") + s);
    return 0;
}
void memCheckHeapReal__FPCci(const char* where, int line) { events.push_back(std::string("heap:") + where + ":" + std::to_string(line)); }
int initFile__FPcPvi(char* name, void* data, int size) {
    events.push_back(std::string("file:") + name + ":" + std::to_string(norrath_host_address(data) - kVirtualBase) + ":" + std::to_string(size));
    return initFileResults[initFileIndex++ % initFileResults.size()];
}
void animMapAnimationSounds__FP15AnimationHeader(void* data) {
    events.push_back("anim:" + std::to_string(norrath_host_address(data) - kVirtualBase));
}
}
static void reset(unsigned count) {
    std::memset(lumpMemory, 0, sizeof lumpMemory);
    *reinterpret_cast<unsigned short*>(lumpMemory) = static_cast<unsigned short>(count);
    events.clear(); initFileIndex = 0; nullStores = 0;
}
static unsigned names = 0x400;
static void row(unsigned index, const char* name, unsigned data, unsigned size) {
    unsigned at = names; names += 32;
    std::strcpy(reinterpret_cast<char*>(lumpMemory + at), name);
    unsigned* r = reinterpret_cast<unsigned*>(lumpMemory + 4 + 12 * index);
    r[0] = at; r[1] = data; r[2] = size;
}
static unsigned* rowp(unsigned index) { return reinterpret_cast<unsigned*>(lumpMemory + 4 + 12 * index); }
static const char* nameOf(unsigned index) { return reinterpret_cast<const char*>(lumpMemory + (rowp(index)[0] - kVirtualBase)); }
static bool is(unsigned i, const std::string& e) { return i < events.size() && events[i] == e; }

int main() {
    // 1. Two fresh rows: prefix stripped, files started in order, only .anm gets sound mapping, shift rounds to 128.
    reset(2); names = 0x400; initFileResults = {0x30, 0x90};
    row(0, "ntsc_a.anm", 0x800, 0x40); row(1, "b.vif", 0x900, 0x40);
    int r = initLump__FPv(lumpMemory);
    if (r != ((0x30 & -128) + 0x90) / 128 * 128) return 1;
    if (!is(0, "heap:lump.cc:298") || !is(1, "file:a.anm:2048:64") || !is(2, "file:b.vif:2304:64") ||
        !is(3, "anim:2048") || !is(4, "heap:lump.cc:400") || events.size() != 5) return 2;
    if (std::strcmp(nameOf(0), "a.anm") != 0 || rowp(1)[1] != kVirtualBase + 0x900) return 3;
    // 2. Unique pal_ row is dropped: later rows slide down, count shrinks, shift grows by its size.
    reset(3); names = 0x400; initFileResults = {0, 0};
    row(0, "a.anm", 0x800, 0x40); row(1, "pal_p.anm", 0x900, 0x100); row(2, "c.anm", 0xa00, 0x40);
    r = initLump__FPv(lumpMemory);
    if (*reinterpret_cast<unsigned short*>(lumpMemory) != 2) return 4;
    if (std::strcmp(nameOf(1), "c.anm") != 0) return 5;
    if (!is(1, "file:a.anm:2048:64") || !is(2, "file:c.anm:2304:64")) return 6;   // c.anm was moved down 0x100
    if (r != 0x100) return 7;                                                   // shift 0x100 from the drop, initFile adds 0
    // 3. pal_ row whose data a later row shares is kept with the prefix stripped; the later row is skipped
    //    because its data is not above the previous one.
    reset(2); names = 0x400; initFileResults = {0, 0};
    row(0, "pal_a.vif", 0x800, 0x40); row(1, "a.vif", 0x800, 0x40);
    initLump__FPv(lumpMemory);
    if (*reinterpret_cast<unsigned short*>(lumpMemory) != 2 || std::strcmp(nameOf(0), "a.vif") != 0) return 8;
    if (events.size() != 3 || !is(1, "file:a.vif:2048:64")) return 12;
    // 4. Rows not above the previous data address are skipped (but converted).
    reset(2); names = 0x400; initFileResults = {0, 0};
    row(0, "a.vif", 0x900, 0x40); row(1, "b.vif", 0x800, 0x40);
    initLump__FPv(lumpMemory);
    if (events.size() != 3 || rowp(1)[1] != kVirtualBase + 0x800) return 9;
    // 5. Zero size reports the name and stores zero through address 0.
    reset(1); names = 0x400; initFileResults = {0};
    row(0, "z.vif", 0x800, 0);
    initLump__FPv(lumpMemory);
    if (!is(1, "printf:z.vif") || nullStores != 1) return 10;
    // 6. Empty lump: only the two heap checks.
    reset(0); initFileResults = {0};
    r = initLump__FPv(lumpMemory);
    if (r != 0 || events.size() != 2) return 11;
    return 0;
}
