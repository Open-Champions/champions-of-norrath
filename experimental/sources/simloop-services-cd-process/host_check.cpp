#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" unsigned char cdBusy = 0;
extern "C" unsigned char streaming = 0;
extern "C" int streamSizeLeft = 0;
extern "C" unsigned char* streamOutput = nullptr;
extern "C" unsigned cdResult = 0;
extern "C" unsigned char lumpLoadLump[4] = {};
extern "C" unsigned char lumpLoadCompletionFlag = 0;
extern "C" unsigned char lumpLoadSize[4] = {};
extern "C" unsigned char lumpLoadName[32] = {};
extern "C" unsigned char lumps[80] = {};
extern "C" unsigned nmLumps = 0;

static int sync_result = 0;
static int error_result = 0;
static int issues = 0;
static int reads = 0;
static int read_blocks = 0;
static int read_error_at_entry = -1;
static int free_values[2] = {500, 420};
static int free_calls = 0;
static int init_result = 64;
static int realloc_size = 0;
static void* realloc_pointer = nullptr;

extern "C" int sceCdSync(int mode) {
    if (mode != 1) std::abort();
    return sync_result;
}
extern "C" int sceCdGetError() { return error_result; }
extern "C" int sceCdStRead(int blocks, void*, int mode, int* error) {
    reads += 1;
    read_blocks = blocks;
    read_error_at_entry = *error;
    if (mode != 0) std::abort();
    return 2;
}
extern "C" void issueCommand__Fv() { issues += 1; }
extern "C" int soundGetFreeRam__Fv() { return free_values[free_calls++]; }
extern "C" int initLump__FPv(void*) { return init_result; }
extern "C" void* memRealloc__FPvi(void* lump, int size) {
    realloc_pointer = lump;
    realloc_size = size;
    return lump;
}
extern "C" char* strcpy(char* destination, const char* source) {
    char* cursor = destination;
    while ((*cursor++ = *source++) != 0) {}
    return destination;
}
extern "C" void cdProcess__Fv();

static void set_word(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}
static unsigned stored_address = 0;
static unsigned char stored_value = 0;
static int stores = 0;
extern "C" void norrath_host_store_byte(unsigned address, unsigned char value) {
    stored_address = address;
    stored_value = value;
    stores += 1;
}
static int fail(int code) {
    std::fprintf(stderr, "cd process failed %d\n", code);
    return code;
}

int main() {
    unsigned char buffer[8192];
    cdResult = 0x1000u;
    cdBusy = 1;
    streaming = 0;
    sync_result = 1;
    cdProcess__Fv();
    if (issues != 0 || stores != 0 || cdBusy != 1) return fail(1);

    sync_result = 0;
    error_result = 4;
    cdProcess__Fv();
    if (issues != 1 || cdBusy != 1 || stores != 0) return fail(2);

    error_result = 0;
    cdProcess__Fv();
    if (stores != 1 || stored_address != 0x1000u || stored_value != 1 || cdBusy != 0) return fail(3);

    cdBusy = 1;
    streaming = 1;
    streamSizeLeft = 4096;
    streamOutput = buffer;
    cdProcess__Fv();
    if (reads != 1 || read_blocks != 2 || read_error_at_entry != 0) return fail(4);
    if (streamSizeLeft != 0 || streamOutput != buffer + 4096 || stores != 2 || cdBusy != 0) return fail(5);

    unsigned char lump[8] = {};
    set_word(lumpLoadLump, static_cast<unsigned>(reinterpret_cast<unsigned long>(lump)));
    set_word(lumpLoadSize, 1000);
    lumpLoadCompletionFlag = 1;
    std::memcpy(lumpLoadName, "globsnd.lmp", 12);
    cdBusy = 0;
    cdProcess__Fv();
    const auto lump_word = static_cast<unsigned>(reinterpret_cast<unsigned long>(lump));
    if (realloc_size != 1000 - 64 + 256 || realloc_pointer != reinterpret_cast<void*>(static_cast<unsigned long>(lump_word)))
        return fail(6);
    if (word(lumps + 24) != static_cast<unsigned>(realloc_size)) return fail(7);
    if (word(lumps + 28) != 80u || word(lumps + 20) != 0u || word(lumps + 32) != 0u) return fail(8);
    if (std::memcmp(lumps, "globsnd.lmp", 12) != 0) return fail(9);
    if (nmLumps != 1 || word(lumpLoadLump) != 0 || lumpLoadCompletionFlag != 0) return fail(10);
    if (word(lumps + 16) != lump_word) return fail(11);
    if (word(lumps + 36) != 1u) return fail(12);
    std::puts("cd process contract passed");
    return 0;
}
