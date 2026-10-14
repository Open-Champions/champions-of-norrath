#include <cstdio>
#include <cstring>
#include "pk_terminate.cpp"
int main() {
    alignas(16) unsigned buffer[16];
    unsigned char* b = reinterpret_cast<unsigned char*>(buffer);
    // Cursor 4 bytes past a quadword, tag at the start: three padding words; header gains 32/16 - 1.
    std::memset(buffer, 0xaa, sizeof buffer);
    buffer[0] = 1;
    GifPacketState packet = {b + 20, b, b};
    void* r = sceGifPkTerminate(&packet);
    if (r != b + 32 || packet.cursor != b + 32 || packet.tag != nullptr || packet.start != b) return 1;
    if (buffer[0] != 1 + (32 >> 4) - 1) return 2;
    if (buffer[5] != 0 || buffer[6] != 0 || buffer[7] != 0 || buffer[4] != 0xaaaaaaaau || buffer[8] != 0xaaaaaaaau) return 3;
    // Already aligned, no tag: nothing written, cursor unchanged.
    std::memset(buffer, 0xaa, sizeof buffer);
    packet = {b + 16, b, nullptr};
    if (sceGifPkTerminate(&packet) != b + 16 || buffer[4] != 0xaaaaaaaau || buffer[3] != 0xaaaaaaaau) return 4;
    // Offset of 4 within a quadword needs three words; of 8 needs two; of 12 one.
    for (int off = 1; off < 4; ++off) {
        std::memset(buffer, 0xaa, sizeof buffer);
        packet = {b + 16 + off * 4, b, nullptr};
        if (sceGifPkTerminate(&packet) != b + 32) return 5;
        for (int w = 4 + off; w < 8; ++w) if (buffer[w] != 0) return 6;
        if (buffer[3 + off] != 0xaaaaaaaau) return 7;
    }
    // Tag later in the buffer: header word accumulates the quadword count - 1.
    std::memset(buffer, 0, sizeof buffer);
    buffer[4] = 0x8000;
    packet = {b + 48, b, b + 16};
    sceGifPkTerminate(&packet);
    if (buffer[4] != 0x8000 + 2 - 1 || packet.cursor != b + 48 || packet.tag != nullptr) return 8;
    std::puts("sceGifPkTerminate contract passed");
    return 0;
}
