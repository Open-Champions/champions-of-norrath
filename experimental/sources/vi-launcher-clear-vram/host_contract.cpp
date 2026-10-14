#define NORRATH_HOST_CONTRACT
#define printf norrath_host_printf
#define exit norrath_host_exit
#include "clear_vram.cpp"
#undef printf
#undef exit

#include <cstdarg>
#include <cstdio>
#include <cstring>

extern "C" void* dmaGIF = nullptr;

static int syncs;
static int sync_mode_bad;
static unsigned long long stores[2];
static unsigned store_address[2];
static int store_count;
static int chan_id;
static void* channel = reinterpret_cast<void*>(0x50);
static void* replacement = reinterpret_cast<void*>(0x60);
static int align_alignment;
static unsigned align_size;
static int dma_ready;
static int fail_alloc;
static void* allocated = reinterpret_cast<void*>(0x70);
static void* init_buffer;
static GifPacket* init_packet;
static int resets;
static unsigned long long added[24];
static int add_count;
static int z_index;
static int z_width;
static int z_height;
static int flushes;
static int flush_mode = -1;
static void* sent_channel;
static void* sent_packet;
static int sent_qwc;
static int path_mode = -1;
static int path_timeout = -1;
static void* freed;
static int free_calls;
static char printed[128];
static int print_calls;
static int exit_code = 99;
static int exit_calls;

extern "C" void norrath_host_store64(unsigned address, unsigned long long value) {
    if (store_count < 2) {
        store_address[store_count] = address;
        stores[store_count] = value;
    }
    ++store_count;
}
extern "C" void sceGsSyncV(int mode) {
    if (mode != 0) sync_mode_bad = 1;
    ++syncs;
}
extern "C" void* sceDmaGetChan(int identifier) {
    chan_id = identifier;
    return channel;
}
extern "C" void* __wrap_memalign(unsigned alignment, unsigned size) {
    align_alignment = static_cast<int>(alignment);
    align_size = size;
    dma_ready = dmaGIF == channel;
    if (fail_alloc) return nullptr;
    return allocated;
}
extern "C" int norrath_host_printf(const char* text, ...) {
    va_list args;
    va_start(args, text);
    std::vsnprintf(printed, sizeof printed, text, args);
    va_end(args);
    ++print_calls;
    return 0;
}
extern "C" void norrath_host_exit(int code) {
    exit_code = code;
    ++exit_calls;
}
extern "C" void sceGifPkInit(GifPacket* packet, void* buffer) {
    init_packet = packet;
    init_buffer = buffer;
}
extern "C" void sceGifPkReset(GifPacket* packet) {
    if (packet == init_packet) ++resets;
}
extern "C" void sceGifPkAddGsData(GifPacket* packet, unsigned long long value) {
    if (packet != init_packet) value = 0;
    if (add_count < 24) added[add_count] = value;
    ++add_count;
}
extern "C" unsigned sceGszbufaddr(int index, int width, int height) {
    z_index = index;
    z_width = width;
    z_height = height;
    return 0x10u;
}
extern "C" void sceGifPkTerminate(GifPacket* packet) {
    if (packet != init_packet) add_count = -1;
}
extern "C" void FlushCache(int mode) {
    ++flushes;
    flush_mode = mode;
}
extern "C" int sceGifPkSize(GifPacket* packet) {
    packet->second = 0xf00000abu;
    dmaGIF = replacement;
    return 9;
}
extern "C" void sceDmaSendN(void* dma_channel, void* packet, int qwc) {
    sent_channel = dma_channel;
    sent_packet = packet;
    sent_qwc = qwc;
}
extern "C" void sceGsSyncPath(int mode, int timeout) {
    path_mode = mode;
    path_timeout = timeout;
}
extern "C" void __wrap_free(void* pointer) {
    ++free_calls;
    freed = pointer;
}

int main() {
    static const unsigned long long expect[] = {
        0x100000000000800aull, 14ull, 1ull, 26ull, 0x100000ull, 76ull, 0x131000010ull, 78ull, 0ull, 24ull,
        0x40004000000ull, 64ull, 0x30000ull, 71ull, 6ull, 0ull, 0x3f80000004030201ull, 1ull, 0ull, 4ull,
        0x40004000ull, 4ull,
    };
    ClearVRAM__FUcUcUcUc(0x101u, 0x202u, 0x303u, 0x404u);
    if (store_count != 2 || store_address[0] != 0x12000000u || stores[0] != 160ull) return 1;
    if (store_address[1] != 0x12000000u || stores[1] != 161ull) return 2;
    if (syncs != 10 || sync_mode_bad != 0) return 3;
    if (chan_id != 2 || dma_ready != 1 || align_alignment != 64 || align_size != 240u) return 4;
    if (init_buffer != allocated || resets != 1 || add_count != 22) return 5;
    for (int index = 0; index < 22; ++index) {
        if (added[index] != expect[index]) return 20 + index;
    }
    if (z_index != 0 || z_width != 640 || z_height != 224) return 6;
    if (flushes != 1 || flush_mode != 0) return 7;
    if (sent_channel != replacement || sent_packet != reinterpret_cast<void*>(0xabu) || sent_qwc != 9) return 8;
    if (path_mode != 0 || path_timeout != 0 || freed != allocated || free_calls != 1) return 9;
    if (print_calls != 0 || exit_calls != 0) return 10;

    fail_alloc = 1;
    syncs = 0;
    store_count = 0;
    add_count = 0;
    free_calls = 0;
    ClearVRAM__FUcUcUcUc(0, 0, 0, 0);
    if (print_calls != 1 || exit_calls != 1 || exit_code != 0) return 11;
    if (std::strcmp(printed, "ERROR: Not enough memory! line:(50), file:(../ee_support_dll/viglobalee.cpp)\n") != 0)
        return 12;
    if (add_count != 22 || freed != nullptr || free_calls != 1) return 13;
    if (added[16] != 0x3f80000000000000ull) return 14;
    return 0;
}
