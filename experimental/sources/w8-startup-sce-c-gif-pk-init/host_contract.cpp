#include <cstdio>
#include "pk_init.cpp"
int main() {
    alignas(16) unsigned char buffer[64];
    unsigned char other[16];
    GifPacketState packet = {other, other + 3, other + 5};
    sceGifPkInit(&packet, buffer);
    if (packet.cursor != buffer || packet.start != buffer || packet.tag != nullptr) return 1;
    sceGifPkInit(&packet, nullptr);
    if (packet.cursor != nullptr || packet.start != nullptr || packet.tag != nullptr) return 2;
    std::puts("sceGifPkInit contract passed");
    return 0;
}
