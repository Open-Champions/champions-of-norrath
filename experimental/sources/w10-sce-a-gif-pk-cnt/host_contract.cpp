// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pk_cnt.cpp
#include <cstring>
struct GifPacketState { unsigned char* cursor; unsigned char* start; unsigned char* dma_tag; unsigned char* gif_tag; };
alignas(16) static unsigned char area[256];
static int terminate_calls; static GifPacketState* terminate_arg; static int terminate_advance = 16;
extern "C" {
void* sceGifPkTerminate(GifPacketState* packet) {     // stand-in: moves the cursor and returns the new one
    ++terminate_calls; terminate_arg = packet;
    packet->cursor += terminate_advance;
    return packet->cursor;
}
void* sceGifPkCnt(GifPacketState* packet, unsigned second, unsigned third, unsigned fourth);
}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }
int main() {
    std::memset(area, 0x99, sizeof area);
    GifPacketState state = {area + 4, area, nullptr, area + 12};
    void* const r = sceGifPkCnt(&state, 0x11111111u, 0x22222222u, 0x00000123u);
    if (terminate_calls != 1 || terminate_arg != &state) return 1;
    // the tag goes where the cursor stood after the terminate call (area + 20)
    if (word(area + 20) != 0x10000123u || word(area + 24) != 0 || word(area + 28) != 0x11111111u || word(area + 32) != 0x22222222u) return 2;
    if (state.cursor != area + 36 || r != area + 36) return 3;                // cursor advances 16 and is returned
    if (state.dma_tag != area + 20) return 4;                                  // the terminate result becomes the open tag
    if (state.start != area || state.gif_tag != area + 12) return 5;
    if (area[19] != 0x99 || area[36] != 0x99) return 6;                        // exactly 16 bytes
    // the fourth argument is OR-ed, not added, and only that one gets the flag
    terminate_advance = 0;
    sceGifPkCnt(&state, 0, 0, 0x10000001u);
    if (word(area + 36) != 0x10000001u) return 7;
    sceGifPkCnt(&state, 0xffffffffu, 0xeeeeeeeeu, 0xffffffffu);
    if (word(area + 52) != 0xffffffffu || word(area + 56) != 0 || word(area + 60) != 0xffffffffu || word(area + 64) != 0xeeeeeeeeu) return 8;
    if (terminate_calls != 3) return 9;
    return 0;
}
