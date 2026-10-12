#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
extern "C" {
unsigned char norrath_libpad_is_warning_word[4] = {1, 0, 0, 0};
unsigned char norrath_libpad_rpc_clients_view[80] = {};
unsigned char norrath_libpad_port_info_view[240] = {};
unsigned char norrath_libpad_dma_info_view[256] = {};
unsigned char norrath_libpad_rpc_buffer_view[128] = {};
}
static int prints = 0;
static char last_print[128];
static int rpc_calls = 0;
static int rpc_result = 0;
static unsigned rpc_seen_buffer;
extern "C" int scePrintf(const char* format, ...) {
    std::va_list args;
    va_start(args, format);
    std::vsnprintf(last_print, sizeof last_print, format, args);
    va_end(args);
    prints += 1;
    return 0;
}
extern "C" void* memset(void* destination, int value, unsigned long size) {
    if (value != 255 || size != 32) std::abort();
    unsigned char* bytes = static_cast<unsigned char*>(destination);
    for (unsigned long index = 0; index < size; ++index) bytes[index] = 0xff;
    return destination;
}
extern "C" int sceSifCallRpc(void* client, unsigned command, unsigned mode, void* send, int send_size, void* receive,
                             int receive_size, void* end, void* end_param) {
    rpc_calls += 1;
    if (client != norrath_libpad_rpc_clients_view || command != 1 || mode != 0) return -1;
    if (send != norrath_libpad_rpc_buffer_view || receive != send || send_size != 128 || receive_size != 128) return -1;
    if (end != nullptr || end_param != nullptr) return -1;
    rpc_seen_buffer = norrath_libpad_rpc_buffer_view[16] | (norrath_libpad_rpc_buffer_view[17] << 8) |
                      (norrath_libpad_rpc_buffer_view[18] << 16) | (norrath_libpad_rpc_buffer_view[19] << 24);
    norrath_libpad_rpc_buffer_view[12] = 0x77;
    norrath_libpad_rpc_buffer_view[20] = 0x55;
    return rpc_result;
}
extern "C" int scePadPortOpen(int port, int slot, void* address);
static unsigned word(const unsigned char* at) { unsigned v; std::memcpy(&v, at, 4); return v; }
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<unsigned long>(p)); }
static void fill(unsigned char* area) { for (unsigned i = 0; i < 512; ++i) area[i] = 0x5a; }
int main() {
    alignas(64) static unsigned char area[512];
    fill(area);
    // misaligned, warning on and off
    if (scePadPortOpen(0, 0, area + 4) != 0 || prints != 1 || std::strncmp(last_print, "libpad: buffer addr is not 64 byte align. ", 42) != 0) return 1;
    norrath_libpad_is_warning_word[0] = 0;
    if (scePadPortOpen(0, 0, area + 32) != 0 || prints != 1 || rpc_calls != 0) return 2;
    norrath_libpad_is_warning_word[0] = 1;
    // already open
    unsigned char* info = norrath_libpad_port_info_view + 1 * 28 + 1 * 112;
    info[16] = 1;
    if (scePadPortOpen(1, 1, area) != 0 || prints != 2 || std::strcmp(last_print, "libpad: pad port is already open [1][1]\n") != 0 || rpc_calls != 0) return 3;
    // RPC failure leaves PadInfo alone
    info[16] = 0;
    rpc_result = -1;
    if (scePadPortOpen(1, 1, area) != 0 || rpc_calls != 1 || info[16] != 0) return 4;
    if (area[103] != 0 || area[0] != 0xff || area[31] != 0xff || area[32] != 0x5a || area[112] != 5 || area[113] != 2) return 5;
    if (area[128 + 112] != 5 || area[128 + 113] != 2 || area[128] != 0xff || area[256] != 0x5a) return 6;
    // success
    fill(area);
    rpc_result = 0;
    int got = scePadPortOpen(1, 1, area);
    if (got != 0x77) return 7;
    if (word(norrath_libpad_rpc_buffer_view) != 1 || word(norrath_libpad_rpc_buffer_view + 4) != 1 || word(norrath_libpad_rpc_buffer_view + 8) != 1) return 8;
    if (rpc_seen_buffer != low(area)) return 9;
    if (word(info) != low(area) || word(info + 4) != low(norrath_libpad_dma_info_view + 32 + 128) || word(info + 8) != 0x55 || word(info + 12) != 0 || word(info + 16) != 1) return 10;
    if (word(area + 88) != 0 || word(area + 96) != 0 || word(area + 128 + 88) != 0 || word(area + 128 + 96) != 0) return 11;
    std::puts("pad port open contract passed");
    return 0;
}
