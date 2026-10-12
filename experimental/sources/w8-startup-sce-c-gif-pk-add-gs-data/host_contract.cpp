#include <cstdio>
#include "pk_add_gs_data.cpp"
int main() {
    unsigned buffer[8] = {};
    GifPacketState packet = {reinterpret_cast<unsigned char*>(buffer), reinterpret_cast<unsigned char*>(buffer), nullptr};
    sceGifPkAddGsData(&packet, 0x1122334455667788ull);
    if (buffer[0] != 0x55667788u || buffer[1] != 0x11223344u || buffer[2] != 0) return 1;
    if (packet.cursor != reinterpret_cast<unsigned char*>(buffer + 2) || packet.start != reinterpret_cast<unsigned char*>(buffer)) return 2;
    sceGifPkAddGsData(&packet, 0xffffffff80000001ull);
    if (buffer[2] != 0x80000001u || buffer[3] != 0xffffffffu || buffer[4] != 0) return 3;
    if (packet.cursor != reinterpret_cast<unsigned char*>(buffer + 4)) return 4;
    sceGifPkAddGsData(&packet, 0);
    if (buffer[4] != 0 || buffer[5] != 0 || packet.cursor != reinterpret_cast<unsigned char*>(buffer + 6)) return 5;
    std::puts("sceGifPkAddGsData contract passed");
    return 0;
}
