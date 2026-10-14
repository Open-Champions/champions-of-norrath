#include <cstdio>
#include "pk_reset.cpp"
int main() {
    alignas(16) unsigned char buffer[64];
    GifPacketState packet = {buffer + 48, buffer, buffer + 16};
    sceGifPkReset(&packet);
    if (packet.cursor != buffer || packet.start != buffer || packet.tag != nullptr) return 1;
    std::puts("sceGifPkReset contract passed");
    return 0;
}
