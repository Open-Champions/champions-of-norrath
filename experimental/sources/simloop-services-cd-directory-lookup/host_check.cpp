#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdint>

extern "C" unsigned char layer0Dir[24580] = {};
extern "C" unsigned char layer1Dir[24580] = {};
extern "C" unsigned char* currentLayerDir = layer0Dir;
extern "C" unsigned isDual = 0;
extern "C" unsigned dualLayer2Start = 1000;
extern "C" unsigned char CDmode[4] = {0, 9, 0, 0};

static int waits = 0;
static int searches = 0;
static int search_layer = -1;
static const char* search_name = nullptr;
static int loads = 0;
static int load_sector = 0;
static int load_blocks = 0;
static unsigned char* loaded = nullptr;

extern "C" int sprintf(char* destination, const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    const int wrote = std::vsnprintf(destination, 80, format, args);
    va_end(args);
    return wrote;
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" void cdWait__Fv() { waits += 1; }
extern "C" int sceCdLayerSearchFile(void* info, const char* name, int layer) {
    searches += 1;
    search_layer = layer;
    search_name = name;
    auto* words = static_cast<unsigned*>(info);
    words[0] = 50u;
    words[1] = 100u;
    return 1;
}
extern "C" void loadDirectory__FP9directoryii(unsigned char* directory, int sector, int blocks) {
    loads += 1;
    loaded = directory;
    load_sector = sector;
    load_blocks = blocks;
    std::memset(directory, 0, 24580);
    std::memcpy(directory, "OTHER.BIN;1", 12);
    directory[16] = 7;
    directory[20] = 9;
    directory[24576] = 1;
}
extern "C" int cdDirectoryLookup__FPcPiT1i(char*, int*, int*, int);

static void set_row(unsigned char* directory, const char* name, unsigned sector, unsigned size, unsigned count) {
    std::memset(directory, 0, 24580);
    std::memcpy(directory, name, std::strlen(name) + 1);
    directory[16] = static_cast<unsigned char>(sector);
    directory[17] = static_cast<unsigned char>(sector >> 8);
    directory[18] = static_cast<unsigned char>(sector >> 16);
    directory[19] = static_cast<unsigned char>(sector >> 24);
    directory[20] = static_cast<unsigned char>(size);
    directory[21] = static_cast<unsigned char>(size >> 8);
    directory[22] = static_cast<unsigned char>(size >> 16);
    directory[23] = static_cast<unsigned char>(size >> 24);
    directory[24576] = static_cast<unsigned char>(count);
    directory[24577] = static_cast<unsigned char>(count >> 8);
    directory[24578] = static_cast<unsigned char>(count >> 16);
    directory[24579] = static_cast<unsigned char>(count >> 24);
}

static bool same(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return *left == *right;
}

static int fail(int code) {
    std::fprintf(stderr, "directory lookup failed %d\n", code);
    return code;
}

int main() {
    set_row(layer0Dir, "PICCACHE.HDR;1", 40, 3000, 1);
    int sector = 0;
    int size = 0;
    char name[] = "piccache.hdr";
    if (cdDirectoryLookup__FPcPiT1i(name, &sector, &size, 7) != 1) return fail(1);
    if (sector != 40 || size != 3000 || waits != 0 || searches != 0) return fail(2);
    int ignored = 5;
    if (cdDirectoryLookup__FPcPiT1i(name, nullptr, &ignored, 0) != 1 || ignored != 3000) return fail(3);
    char missing[] = "nope";
    if (cdDirectoryLookup__FPcPiT1i(missing, &sector, &size, 0) != 0) return fail(4);

    isDual = 1;
    CDmode[1] = 9;
    currentLayerDir = layer0Dir;
    set_row(layer0Dir, "STAY.BIN;1", 1, 2, 1);
    set_row(layer1Dir, "", 0, 0, 0);
    sector = 0;
    size = 0;
    char other[] = "other.bin";
    if (cdDirectoryLookup__FPcPiT1i(other, &sector, &size, 0) != 1) return fail(5);
    if (waits != 1 || searches != 1 || search_layer != 1) return fail(6);
    if (!same(search_name, "\\BG\\DATA")) return fail(7);
    if (loads != 1 || loaded != layer1Dir || load_sector != 50 || load_blocks != ((100u + 2047u) >> 11)) return fail(8);
    if (currentLayerDir != layer1Dir || CDmode[1] != 0) return fail(9);
    if (sector != 1007 || size != 9) return fail(10);

    waits = 0;
    searches = 0;
    loads = 0;
    currentLayerDir = layer1Dir;
    set_row(layer0Dir, "BACK.BIN;1", 3, 4, 1);
    if (cdDirectoryLookup__FPcPiT1i(const_cast<char*>("back.bin"), &sector, &size, 0) != 1) return fail(11);
    if (search_layer != 0 || loads != 0 || currentLayerDir != layer0Dir || sector != 3) return fail(12);
    std::puts("directory lookup contract passed");
    return 0;
}
