#include <cstdio>
#include <cstring>
#include "pk_end.cpp"
static int terminate_calls;
static unsigned char* terminate_pad = nullptr;
extern "C" void* sceGifPkTerminate(GifPacketState* packet) {
    ++terminate_calls;
    if (terminate_pad) packet->cursor = terminate_pad;
    return packet->cursor;
}
int main() {
    alignas(16) unsigned buffer[16];
    std::memset(buffer, 0xaa, sizeof buffer);
    unsigned char* b = reinterpret_cast<unsigned char*>(buffer);
    GifPacketState packet = {b + 16, b, nullptr};
    terminate_pad = b + 32;   // the terminate step moves the cursor before the end tag is written
    sceGifPkEnd(&packet, 0x11, 0x22, 5);
    if (terminate_calls != 1) return 1;
    if (buffer[8] != (0x70000000u | 5) || buffer[9] != 0 || buffer[10] != 0x11 || buffer[11] != 0x22) return 2;
    if (packet.cursor != b + 48 || packet.tag != b + 32 || packet.start != b) return 3;
    if (buffer[7] != 0xaaaaaaaau || buffer[12] != 0xaaaaaaaau) return 4;
    std::memset(buffer, 0xaa, sizeof buffer);
    terminate_pad = nullptr;
    packet = {b, b, nullptr};
    sceGifPkEnd(&packet, 0, 0, 0);
    if (buffer[0] != 0x70000000u || buffer[1] != 0 || buffer[2] != 0 || buffer[3] != 0 || packet.cursor != b + 16 || packet.tag != b) return 5;
    std::puts("sceGifPkEnd contract passed");
    return 0;
}
