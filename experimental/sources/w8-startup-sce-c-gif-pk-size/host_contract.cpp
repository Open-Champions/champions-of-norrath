#include <cstdio>
#include "pk_size.cpp"
int main() {
    alignas(16) unsigned char buffer[256];
    GifPacketState packet = {buffer, buffer, nullptr};
    if (sceGifPkSize(&packet) != 0) return 1;
    packet.cursor = buffer + 48;
    if (sceGifPkSize(&packet) != 3) return 2;
    packet.cursor = buffer + 47;   // partial quadword truncates
    if (sceGifPkSize(&packet) != 2) return 3;
    packet.cursor = buffer + 255;
    if (sceGifPkSize(&packet) != 15) return 4;
    std::puts("sceGifPkSize contract passed");
    return 0;
}
