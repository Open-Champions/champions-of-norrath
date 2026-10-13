// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pk_reserve.cpp
struct GifPacketState { unsigned char* cursor; unsigned char* start; unsigned char* dma_tag; unsigned char* gif_tag; };
extern "C" void* sceGifPkReserve(GifPacketState* packet, int words);
int main() {
    alignas(16) static unsigned char area[256];
    GifPacketState state = {area + 8, area, area + 4, area + 12};
    if (sceGifPkReserve(&state, 4) != area + 8 || state.cursor != area + 24) return 1;     // returns the old cursor
    if (state.start != area || state.dma_tag != area + 4 || state.gif_tag != area + 12) return 2;
    if (sceGifPkReserve(&state, 0) != area + 24 || state.cursor != area + 24) return 3;
    if (sceGifPkReserve(&state, 1) != area + 24 || state.cursor != area + 28) return 4;    // four bytes per word
    if (sceGifPkReserve(&state, -2) != area + 28 || state.cursor != area + 20) return 5;   // no check on the count
    for (unsigned char c : area) if (c != 0) return 6;                                      // nothing is written
    return 0;
}
