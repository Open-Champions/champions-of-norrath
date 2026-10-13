// Host check for lumpPartialLoad__FPcT0. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -fno-builtin host_check.cpp
// The image's 32-bit pointer words are offsets into one host arena; file reads are served from an
// in-memory lump file. Expected traces, records and results are written out by hand.
#define NORRATH_HOST_ARENA
#include "lump_partial_load.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int nmLumps;
alignas(8) unsigned char lumps[40 * 12];
}
static unsigned char arena[1 << 22];
extern "C" unsigned char* norrath_host_base = arena;

static std::string trace;
static std::vector<unsigned char> fileImage;
static int initResult;
static unsigned char* const blockAddress = arena + 0x100000;
static char findAnswer;
extern "C" void* lumpFindResource__FPcT0(char* lump, char* resource) {
    trace += std::string("find ") + lump + " " + resource + ";";
    return &findAnswer;
}
extern "C" void* memAlloc__FiPc(int size, char* label) {
    char b[64];
    std::snprintf(b, sizeof b, "alloc %d rec%d;", size, static_cast<int>((reinterpret_cast<unsigned char*>(label) - lumps) / 40));
    trace += b;
    std::memset(blockAddress, 0xee, 0x100000);
    return blockAddress;
}
extern "C" void cdLoad__FPcPvii(char* name, void* destination, int size, int offset) {
    char b[96];
    std::snprintf(b, sizeof b, "cd %s +%d %d @%d;", name, static_cast<int>(static_cast<unsigned char*>(destination) - blockAddress), size, offset);
    trace += b;
    for (int i = 0; i < size; ++i) {
        const size_t at = static_cast<size_t>(offset) + static_cast<size_t>(i);
        static_cast<unsigned char*>(destination)[i] = at < fileImage.size() ? fileImage[at] : 0x5a;
    }
}
extern "C" int initFile__FPcPvi(char* name, void* data, int size) {
    char b[96];
    std::snprintf(b, sizeof b, "init %s +%d %d;", name, static_cast<int>(static_cast<unsigned char*>(data) - blockAddress), size);
    trace += b;
    return initResult;
}
extern "C" void memRealloc__FPvi(void* block, int size) {
    char b[64];
    std::snprintf(b, sizeof b, "realloc %d %d;", block == blockAddress, size);
    trace += b;
}
extern "C" void memFree__FPv(void* block) { trace += block == blockAddress ? "free;" : "free?;"; }

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

static void setRecord(int i, const char* name, unsigned flag, unsigned pointerWord) {
    unsigned char* r = lumps + 40 * i;
    std::memset(r, 0, 40);
    std::strcpy(reinterpret_cast<char*>(r), name);
    std::memcpy(r + 16, &pointerWord, 4);
    std::memcpy(r + 20, &flag, 4);
    unsigned filler = 0x7777;
    std::memcpy(r + 24, &filler, 4);
}
static unsigned wordAt(const unsigned char* r, int off) { unsigned v; std::memcpy(&v, r + off, 4); return v; }

struct Res { const char* name; unsigned offset; unsigned size; };

// Builds a lump file: count, 12-byte entries sorted as given, then the names at nameBase.
static void buildFile(const std::vector<Res>& res, unsigned nameBase) {
    fileImage.assign(0x20000, 0x31);
    const unsigned short count = static_cast<unsigned short>(res.size());
    std::memcpy(&fileImage[0], &count, 2);
    unsigned pos = nameBase;
    for (size_t i = 0; i < res.size(); ++i) {
        unsigned char* e = &fileImage[4 + 12 * i];
        std::memcpy(e, &pos, 4);
        std::memcpy(e + 4, &res[i].offset, 4);
        std::memcpy(e + 8, &res[i].size, 4);
        std::strcpy(reinterpret_cast<char*>(&fileImage[pos]), res[i].name);
        pos += static_cast<unsigned>(std::strlen(res[i].name)) + 1;
    }
}

static unsigned wordOfPointer(const void* p) { return static_cast<unsigned>(static_cast<const unsigned char*>(p) - arena); }

int main() {
    char lump[] = "level.lmp";
    char a[] = "a.tex";
    // 1. a loaded lump (flag bit 0 clear, data pointer set): resource lookup is handed on
    nmLumps = 2; trace.clear();
    setRecord(0, "other.lmp", 0, 0x300000);
    setRecord(1, "level.lmp", 0, 0x300000);
    CHECK(lumpPartialLoad__FPcT0(lump, a) == &findAnswer && trace == "find level.lmp a.tex;");
    // the first matching record decides: a first match with a zero pointer does not fall back to a later one
    nmLumps = 2; trace.clear();
    setRecord(0, "level.lmp", 0, 0);
    setRecord(1, "level.lmp", 0, 0x300000);
    buildFile({}, 0x200);
    initResult = 0;
    CHECK(lumpPartialLoad__FPcT0(lump, a) == nullptr);
    CHECK(trace == "alloc 1048576 rec2;cd level.lmp +0 32768 @0;free;");
    // 2. partially loaded resource from an earlier call (flag bit 0 set, header text equals the name)
    nmLumps = 1; trace.clear();
    setRecord(0, "level.lmp", 1, 0x300000);
    std::strcpy(reinterpret_cast<char*>(arena + 0x300000), "a.tex");
    CHECK(lumpPartialLoad__FPcT0(lump, a) == arena + 0x300000 + 256 && trace.empty());
    // a different resource of the same partial record goes on to a new load; a lump of another
    // name does not count; a partial record whose flag has bit 0 clear is a loaded lump instead
    char b[] = "b.tex";
    buildFile({{"a.tex", 0x9000, 100}, {"b.tex", 0xa000, 2049}}, 0x200);
    initResult = 5;
    nmLumps = 1; trace.clear();
    void* r = lumpPartialLoad__FPcT0(lump, b);
    CHECK(r == blockAddress);
    CHECK(trace == "alloc 1048576 rec1;cd level.lmp +0 32768 @0;cd level.lmp +256 4096 @40960;init b.tex +256 4096;realloc 1 4347;");
    CHECK(nmLumps == 2);
    {
        const unsigned char* rec = lumps + 40;
        CHECK(std::strcmp(reinterpret_cast<const char*>(rec), "level.lmp") == 0);
        CHECK(wordAt(rec, 16) == wordOfPointer(blockAddress) && wordAt(rec, 20) == 1 && wordAt(rec, 24) == 4347);
        CHECK(wordAt(rec, 28) == 0 && wordAt(rec, 32) == 0 && wordAt(rec, 36) == 1);
        CHECK(std::strcmp(reinterpret_cast<char*>(blockAddress), "b.tex") == 0);
        CHECK(wordAt(lumps, 16) == 0x300000);          // the old record is untouched
    }
    // the new record answers the same request without any load
    trace.clear();
    CHECK(lumpPartialLoad__FPcT0(lump, b) == blockAddress + 256 && trace.empty());
    // 3. binary search over 1, 2, 3, 5 and 8 entries: every name is found, in-between names are not
    const char* all[] = {"a", "c", "e", "g", "i", "k", "m", "o"};
    const char* miss[] = {"0", "b", "d", "f", "h", "j", "l", "n", "z", ""};
    for (int n : {1, 2, 3, 5, 8}) {
        std::vector<Res> res;
        for (int i = 0; i < n; ++i) res.push_back({all[i], 0x9000u + 0x800u * i, 1000u * (i + 1)});
        for (int i = 0; i < n; ++i) {
            buildFile(res, 0x200);
            nmLumps = 0; trace.clear(); initResult = 100;
            char name[8]; std::strcpy(name, all[i]);
            void* got = lumpPartialLoad__FPcT0(lump, name);
            const unsigned size = (1000u * (i + 1) + 2047u) & ~2047u;
            char expect[200];
            std::snprintf(expect, sizeof expect, "alloc 1048576 rec0;cd level.lmp +0 32768 @0;cd level.lmp +256 %u @%u;init %s +256 %u;realloc 1 %u;",
                          size, 0x9000u + 0x800u * i, all[i], size, size - 100 + 256);
            if (trace != expect) std::printf("n=%d i=%d\n  got  %s\n  want %s\n", n, i, trace.c_str(), expect);
            CHECK(trace == expect && got == blockAddress && nmLumps == 1);
            CHECK(wordAt(lumps, 24) == size - 100 + 256);
        }
        for (const char* m : miss) {
            bool inRange = false;
            for (int i = 0; i < n; ++i) if (std::strcmp(all[i], m) == 0) inRange = true;
            if (inRange) continue;
            buildFile(res, 0x200);
            nmLumps = 0; trace.clear();
            char name[8]; std::strcpy(name, m);
            CHECK(lumpPartialLoad__FPcT0(lump, name) == nullptr);
            CHECK(trace == "alloc 1048576 rec0;cd level.lmp +0 32768 @0;free;" && nmLumps == 0);
        }
    }
    // sizes are rounded up to 2048 only when needed
    {
        const unsigned sizes[] = {0, 1, 2047, 2048, 2049, 4096};
        const unsigned rounded[] = {0, 2048, 2048, 2048, 4096, 4096};
        for (int i = 0; i < 6; ++i) {
            buildFile({{"a.tex", 0x9000, sizes[i]}}, 0x200);
            nmLumps = 0; trace.clear(); initResult = 0;
            lumpPartialLoad__FPcT0(lump, a);
            char expect[200];
            std::snprintf(expect, sizeof expect, "alloc 1048576 rec0;cd level.lmp +0 32768 @0;cd level.lmp +256 %u @36864;init a.tex +256 %u;realloc 1 %u;",
                          rounded[i], rounded[i], rounded[i] + 256);
            CHECK(trace == expect);
        }
    }
    // 4. an empty directory
    buildFile({}, 0x200);
    setRecord(0, "x0.lmp", 0, 0); setRecord(1, "x1.lmp", 1, 0x300000); setRecord(2, "x2.lmp", 0, 0x300000);
    nmLumps = 3; trace.clear();
    CHECK(lumpPartialLoad__FPcT0(lump, a) == nullptr && trace == "alloc 1048576 rec3;cd level.lmp +0 32768 @0;free;");
    // 5. a directory that extends past the first 0x8000 bytes reads the rest, and the count again
    {
        std::vector<Res> res = {{"a.tex", 0x9000, 100}, {"b.tex", 0xa000, 100}};
        buildFile(res, 0x7f90);   // last name offset 0x7f96: (0x7f96 + 2175) & -2048 = 0x8800
        nmLumps = 0; trace.clear(); initResult = 0;
        CHECK(lumpPartialLoad__FPcT0(lump, b) == blockAddress);
        CHECK(trace == "alloc 1048576 rec0;cd level.lmp +0 32768 @0;cd level.lmp +32768 2048 @32768;cd level.lmp +256 2048 @40960;init b.tex +256 2048;realloc 1 2304;");
        // the name pointers were made absolute before the search: the match used the second page's text
        CHECK(std::strcmp(reinterpret_cast<char*>(blockAddress), "b.tex") == 0);
        // exactly 0x8000 as the reach does not trigger a second read
        buildFile(res, 0x7a00);
        nmLumps = 0; trace.clear();
        CHECK(lumpPartialLoad__FPcT0(lump, b) == blockAddress);
        CHECK(trace.find("+32768") == std::string::npos);
    }
    std::puts("lump partial load passed");
    return 0;
}
