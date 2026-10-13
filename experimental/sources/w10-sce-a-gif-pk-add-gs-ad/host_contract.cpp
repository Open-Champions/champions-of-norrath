// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pk_add_gs_ad.cpp
#include <cstring>
struct GifPacketState { unsigned char* cursor; unsigned char* start; unsigned char* dma_tag; unsigned char* gif_tag; };
extern "C" void* sceGifPkAddGsAD(GifPacketState* packet, unsigned address, unsigned long long data);
static unsigned long long qword(const unsigned char* p) { unsigned long long v; std::memcpy(&v, p, 8); return v; }
int main() {
    alignas(16) static unsigned char area[128];
    std::memset(area, 0x99, sizeof area);
    GifPacketState state = {area + 16, area, area + 4, area + 12};
    void* const r = sceGifPkAddGsAD(&state, 0x50, 0x1122334455667788ull);
    if (r != area + 32 || state.cursor != area + 32) return 1;
    if (qword(area + 16) != 0x1122334455667788ull) return 2;                  // data first
    if (qword(area + 24) != 0x50ull) return 3;                                 // then the register value
    if (area[15] != 0x99 || area[32] != 0x99) return 4;                        // exactly 16 bytes
    if (state.start != area || state.dma_tag != area + 4 || state.gif_tag != area + 12) return 5;
    // the register value is zero-extended, not sign-extended
    sceGifPkAddGsAD(&state, 0xffffffffu, 0);
    if (qword(area + 32) != 0 || qword(area + 40) != 0xffffffffull) return 6;
    sceGifPkAddGsAD(&state, 0x80000000u, 0xffffffffffffffffull);
    if (qword(area + 48) != 0xffffffffffffffffull || qword(area + 56) != 0x80000000ull) return 7;
    if (state.cursor != area + 64) return 8;
    return 0;
}
