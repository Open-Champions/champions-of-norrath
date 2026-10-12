#include "get_stream_preload_done.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char stream[3 * 452] = {};
static void setw(int r, int off, int v) { std::memcpy(stream + r * 452 + off, &v, 4); }
int main() {
    // no flagged block: 1, whatever the other words say
    setw(1, 36, 0); 
    if (soundGetStreamPreloadDone__Fi(1) != 1) return 1;
    // flagged first block, small amount, field set: 0
    setw(1, 108, 8); setw(1, 84, 5);
    if (soundGetStreamPreloadDone__Fi(1) != 0) return 2;
    setw(1, 84, 0);                       // field zero: 1
    if (soundGetStreamPreloadDone__Fi(1) != 1) return 3;
    setw(1, 84, 5); setw(1, 36, 0xc800);  // equal to the bound: still 0
    if (soundGetStreamPreloadDone__Fi(1) != 0) return 4;
    setw(1, 36, 0xc801);                  // above: 1
    if (soundGetStreamPreloadDone__Fi(1) != 1) return 5;
    setw(1, 36, -1);                      // signed compare: negative is not above
    if (soundGetStreamPreloadDone__Fi(1) != 0) return 6;
    // first flagged block is block 3; its field is used, not block 0's
    setw(1, 108, 0); setw(1, 108 + 48 * 3, 0x18); setw(1, 84, 0); setw(1, 84 + 48 * 3, 9);
    if (soundGetStreamPreloadDone__Fi(1) != 0) return 7;
    // later flagged blocks are ignored
    setw(1, 108 + 48 * 5, 8); setw(1, 84 + 48 * 5, 0);
    if (soundGetStreamPreloadDone__Fi(1) != 0) return 8;
    // block 7 can be the flagged one
    setw(2, 108 + 48 * 7, 8); setw(2, 84 + 48 * 7, 0);
    if (soundGetStreamPreloadDone__Fi(2) != 1) return 9;
    setw(2, 84 + 48 * 7, 1);
    if (soundGetStreamPreloadDone__Fi(2) != 0) return 10;
    std::puts("get_stream_preload_done passed");
    return 0;
}
