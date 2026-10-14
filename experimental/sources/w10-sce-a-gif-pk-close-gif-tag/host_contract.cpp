// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp pk_close_gif_tag.cpp
#include <cstring>
struct GifPacketState { unsigned char* cursor; unsigned char* start; unsigned char* dma_tag; unsigned char* gif_tag; };
extern "C" void sceGifPkCloseGifTag(GifPacketState* packet);
alignas(16) static unsigned char area[512];
typedef unsigned long long u64;
static u64 qword(const unsigned char* p) { u64 v; std::memcpy(&v, p, 8); return v; }
// Close a tag of the given field/register-count whose data takes `dwords` doublewords; returns the tag's new low doubleword.
static u64 run(unsigned flg, unsigned nreg, u64 extra, int dwords, int cursor_skew, GifPacketState& state) {
    std::memset(area, 0x99, sizeof area);
    unsigned char* tag = area + 32;
    const u64 low = (static_cast<u64>(nreg) << 60) | (static_cast<u64>(flg) << 58) | extra;
    std::memcpy(tag, &low, 8);
    state.cursor = tag + 16 + dwords * 8 + cursor_skew; state.start = area; state.dma_tag = area + 4; state.gif_tag = tag;
    sceGifPkCloseGifTag(&state);
    return qword(tag) - low;
}
int main() {
    GifPacketState s;
    // F = 2: half of the data doublewords
    if (run(2, 7, 0, 10, 0, s) != 5) return 1;
    if (run(2, 7, 0, 11, 0, s) != 5) return 2;                         // rounds down
    // F = 1: ceil(D / M), M = N
    if (run(1, 4, 0, 10, 0, s) != 3) return 3;
    if (run(1, 4, 0, 8, 0, s) != 2) return 4;
    if (run(1, 0, 0, 33, 0, s) != 3) return 5;                          // N = 0 means 16
    if (run(1, 0, 0, 32, 0, s) != 2) return 6;
    // F = 0 and F = 3: ceil((D / 2) / M)
    if (run(0, 3, 0, 14, 0, s) != 3) return 7;                          // 7 / 3 -> 3
    if (run(0, 3, 0, 12, 0, s) != 2) return 8;                          // 6 / 3 -> 2
    if (run(3, 3, 0, 14, 0, s) != 3) return 9;
    if (run(0, 0, 0, 34, 0, s) != 2) return 10;                         // 17 / 16 -> 2
    if (run(0, 0, 0, 32, 0, s) != 1) return 11;                         // 16 / 16 -> 1
    if (run(0, 1, 0, 0, 0, s) != 0) return 12;                          // empty data
    // the count is added to the existing doubleword (here with bits above the count field set)
    {
        const u64 extra = 0x0000123400000000ull | 7;
        run(2, 0, extra, 6, 0, s);
        if (qword(area + 32) != ((2ull << 58) | extra) + 3) return 13;
    }
    // the state's tag word is cleared; the other words stay
    run(2, 0, 0, 4, 0, s);
    if (s.gif_tag != nullptr || s.start != area || s.dma_tag != area + 4) return 14;
    // an odd number of doublewords leaves bit 3 set: zero words pad the cursor to the next 16-byte boundary
    run(2, 0, 0, 5, 0, s);
    if (s.cursor != area + 32 + 16 + 48 || qword(area + 32 + 16 + 40) != 0 || area[32 + 16 + 40 - 1] != 0x99 || area[32 + 16 + 48] != 0x99) return 15;
    // a cursor four bytes past that: padding of three zero words? bits 2 and 3 of the address drive it
    run(2, 0, 0, 4, 4, s);
    if (s.cursor != area + 32 + 16 + 32 + 16 || qword(area + 32 + 16 + 32 + 4) != 0 || area[32 + 16 + 32 + 16] != 0x99) return 16;
    // an already aligned cursor is untouched and nothing past the data is written
    run(2, 0, 0, 4, 0, s);
    if (s.cursor != area + 32 + 16 + 32 || area[32 + 16 + 32] != 0x99) return 17;
    return 0;
}
