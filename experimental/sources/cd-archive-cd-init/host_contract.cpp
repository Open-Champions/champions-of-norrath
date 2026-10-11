#include <cstdio>
#include <cstdint>
extern "C" unsigned char CDmode[4] = {};
extern "C" unsigned char isDual[4] = {};
extern "C" unsigned char dualLayer2Start[4] = {};
extern "C" unsigned char norrath_archive_directory_a_view[8] = {};
extern "C" const unsigned char norrath_archive_directory_name_view[] = {'\\','B','G','\\','D','A','T','A',0};
static int dual_calls = 0;
static unsigned char* dual_arg = nullptr;
static unsigned char* layer_arg = nullptr;
static int mode_during_dual[3] = {};
static int dual_word = 0;
static int layer_word = 0;
static int poke_mode_byte = -1;
static int search_result = 0;
static unsigned search_sector = 0;
static unsigned search_size = 0;
static const unsigned char* search_name = nullptr;
static int loads = 0;
static unsigned char* loaded_directory = nullptr;
static unsigned loaded_sector = 0;
static unsigned loaded_blocks = 0;
static unsigned heap_bytes = 0;
static unsigned heap_result = 0;
static unsigned stream_bufmax = 0;
static unsigned stream_banks = 0;
static unsigned stream_iop = 0;
static void put_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
extern "C" int sceCdReadDvdDualInfo(unsigned char* dual, unsigned char* layer) {
    dual_calls += 1;
    dual_arg = dual;
    layer_arg = layer;
    mode_during_dual[0] = CDmode[0];
    mode_during_dual[1] = CDmode[1];
    mode_during_dual[2] = CDmode[2];
    put_word(dual, static_cast<unsigned>(dual_word));
    put_word(layer, static_cast<unsigned>(layer_word));
    if (poke_mode_byte >= 0) CDmode[1] = static_cast<unsigned char>(poke_mode_byte);
    return 1;
}
extern "C" int sceCdSearchFile(unsigned char* file, const unsigned char* name) {
    search_name = name;
    put_word(file, search_sector);
    put_word(file + 4, search_size);
    return search_result;
}
extern "C" void loadDirectory__FP9directoryii(unsigned char* directory, unsigned sector, unsigned blocks) {
    loads += 1;
    loaded_directory = directory;
    loaded_sector = sector;
    loaded_blocks = blocks;
}
extern "C" void* sceSifAllocIopHeap(unsigned bytes) {
    heap_bytes = bytes;
    return reinterpret_cast<void*>(static_cast<uintptr_t>(heap_result));
}
extern "C" void sceCdStInit(unsigned bufmax, unsigned bankmax, unsigned iop_address) {
    stream_bufmax = bufmax;
    stream_banks = bankmax;
    stream_iop = iop_address;
}
extern "C" void cdInit__Fv();
static void reset() {
    for (int index = 0; index < 4; ++index) {
        CDmode[index] = 0;
        isDual[index] = 0;
        dualLayer2Start[index] = 0;
    }
    CDmode[3] = 0x5a;
    dual_calls = 0;
    loads = 0;
    poke_mode_byte = -1;
    search_result = 0;
    heap_result = 0x1001u;
}
int main() {
    reset();
    dual_word = 0;
    layer_word = 50;
    poke_mode_byte = 9;
    cdInit__Fv();
    if (dual_calls != 1 || dual_arg != isDual || layer_arg != dualLayer2Start) return 1;
    if (mode_during_dual[0] != 0xc8 || mode_during_dual[1] != 0 || mode_during_dual[2] != 0) return 2;
    if (CDmode[0] != 0xc8 || CDmode[1] != 9 || CDmode[2] != 0 || CDmode[3] != 0x5a) return 3;
    if (loads != 0) return 4;
    if (search_name != norrath_archive_directory_name_view) return 5;
    if (heap_bytes != 0x14800u || stream_bufmax != 40u || stream_banks != 3u || stream_iop != 0x1800u) return 6;
    reset();
    dual_word = 0x100;
    poke_mode_byte = 9;
    cdInit__Fv();
    if (CDmode[1] != 0) return 7;
    reset();
    search_result = 1;
    search_sector = 10;
    search_size = 3000;
    heap_result = 0x1800u;
    cdInit__Fv();
    if (loads != 1 || loaded_directory != norrath_archive_directory_a_view) return 8;
    if (loaded_sector != 10u || loaded_blocks != 2u) return 9;
    if (stream_iop != 0x1800u) return 10;
    reset();
    search_result = 1;
    search_size = 0xfffffff0u;
    heap_result = 0x1801u;
    cdInit__Fv();
    if (loaded_blocks != 0u || stream_iop != 0x2000u) return 11;
    std::puts("cd init contract passed");
    return 0;
}
