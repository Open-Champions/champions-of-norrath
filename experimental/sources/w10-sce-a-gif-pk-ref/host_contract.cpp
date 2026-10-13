// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pk_ref.cpp
#include <cstdint>
#include <cstring>
struct GifPacketState { unsigned char* cursor; unsigned char* start; unsigned char* dma_tag; unsigned char* gif_tag; };
alignas(16) static unsigned char area[256];
static int terminate_calls; static GifPacketState* terminate_arg;
extern "C" {
void* sceGifPkTerminate(GifPacketState* packet) {
    ++terminate_calls; terminate_arg = packet;
    packet->cursor += 16;
    return packet->cursor;
}
void sceGifPkRef(GifPacketState* packet, const void* data, unsigned count, unsigned fourth, unsigned fifth, unsigned sixth);
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
static const void* low_pointer(unsigned value) { return reinterpret_cast<const void*>(static_cast<std::uintptr_t>(value)); }
int main() {
    std::memset(area, 0x99, sizeof area);
    GifPacketState state = {area, area, area + 4, area + 12};
    // the entry goes after the terminate call moved the cursor; the data address is read as 32 bits
    sceGifPkRef(&state, low_pointer(0xa0123450u), 0x120, 0xaaaa0001u, 0xbbbb0002u, 0x00cc0000u);
    if (terminate_calls != 1 || terminate_arg != &state) return 1;
    if (word(area + 16) != (0x00cc0000u | 0x120u | 0x30000000u)) return 2;
    if (word(area + 20) != 0x80123450u) return 3;                              // AND 0x9fffffff clears bit 29 here
    if (word(area + 24) != 0xaaaa0001u || word(area + 28) != 0xbbbb0002u) return 4;
    if (state.cursor != area + 32) return 5;
    if (area[15] != 0x99 || area[32] != 0x99) return 6;
    if (state.start != area || state.dma_tag != area + 4 || state.gif_tag != area + 12) return 7;
    // bit 31 is cleared as well, other bits kept
    sceGifPkRef(&state, low_pointer(0xffffffffu), 0, 0, 0, 0);
    if (word(area + 48) != 0x30000000u || word(area + 52) != 0x9fffffffu || word(area + 56) != 0 || word(area + 60) != 0) return 8;
    sceGifPkRef(&state, low_pointer(0x00100000u), 0x7fff, 0, 0, 0);
    if (word(area + 80) != 0x30007fffu || word(area + 84) != 0x00100000u) return 9;
    if (terminate_calls != 3) return 10;
    return 0;
}
