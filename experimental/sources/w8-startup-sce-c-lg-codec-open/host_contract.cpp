#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "codec_open.cpp"
extern "C" alignas(4) unsigned char g_codecs[40] = {};
static int mallocs, creates, options; static unsigned malloc_size; static void* last_block;
static void *option_state; static int option_number, option_seen;
extern "C" void* __wrap_malloc(unsigned size) {
    ++mallocs; malloc_size = size;
    last_block = std::malloc(size);
    std::memset(last_block, 0x99, size);
    return last_block;
}
extern "C" void* gsm_create() { ++creates; return reinterpret_cast<void*>(0x1000 * creates); }
extern "C" int gsm_option(void* state, int option, int* value) { ++options; option_state = state; option_number = option; option_seen = *value; return 0; }
static void put(int offset, unsigned value) { std::memcpy(g_codecs + offset, &value, 4); }
int main() {
    for (int index = 0; index < 40; ++index) g_codecs[index] = static_cast<unsigned char>(index);
    put(0, 300); put(20, 7);
    unsigned char out[4] = {1, 2, 3, 4};
    // Unknown identifier.
    if (lgCodecOpen(8, out) != 0x80000004u || mallocs != 0 || out[0] != 1) return 1;
    if (lgCodecOpen(-1, out) != 0x80000004u) return 2;
    // Second record matches but is not 300: nothing is allocated or written.
    if (lgCodecOpen(7, out) != 0u || mallocs != 0 || out[0] != 1) return 3;
    // First record, 300.
    if (lgCodecOpen(300, out) != 0u || mallocs != 1 || malloc_size != 392u) return 4;
    unsigned char* block = static_cast<unsigned char*>(last_block);
    unsigned stored; std::memcpy(&stored, out, 4);
    if (stored != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(block))) return 5;
    unsigned w;
    std::memcpy(&w, block + 0x14, 4); if (w != 0x1000u) return 6;
    std::memcpy(&w, block + 0x18, 4); if (w != 0x2000u) return 7;
    if (creates != 2 || options != 1 || option_state != reinterpret_cast<void*>(0x1000) || option_number != 3 || option_seen != 1) return 8;
    // The first 20 bytes are the record; the two library words and zeros follow.
    if (std::memcmp(block, g_codecs, 20) != 0) return 9;
    for (int index = 0x1c; index < 392; ++index) if (block[index] != 0) return 11;
    std::free(last_block);
    // 300 in the second record copies that record.
    put(0, 5); put(20, 300);
    if (lgCodecOpen(300, out) != 0u || mallocs != 2) return 12;
    if (std::memcmp(last_block, g_codecs + 20, 20) != 0) return 13;
    std::free(last_block);
    std::puts("lgCodecOpen contract passed");
    return 0;
}
