// Host contract for lightSetupAmbientOnly__FP6Packet. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* last; };
extern "C" {
short charAmbient[4];
void lightSetupAmbientOnly__FP6Packet(Packet*);
}
int main() {
    alignas(16) unsigned char out[256];
    int failures = 0;
    std::memset(out, 0xa5, sizeof out);
    charAmbient[0] = 0x1234; charAmbient[1] = -2; charAmbient[2] = 300; charAmbient[3] = -32768;
    Packet packet{out + 16, out + 8};
    lightSetupAmbientOnly__FP6Packet(&packet);
    // 104 bytes are written from the old cursor: tag word, then 100 bytes
    unsigned char want[104];
    std::memset(want, 0, sizeof want);
    auto put32 = [&](int off, unsigned v) { std::memcpy(want + off, &v, 4); };
    auto put16 = [&](int off, short v) { std::memcpy(want + off, &v, 2); };
    put32(0, 0x6c03c000u);
    put32(4 + 48, 0x14000010u);
    put32(4 + 52, 0x6d058000u);
    put32(4 + 96, 0x14000024u);
    for (int i = 0; i < 4; ++i) put16(4 + 88 + 2 * i, charAmbient[i]);
    if (std::memcmp(out + 16, want, 104) != 0) { std::fprintf(stderr, "fail: packet bytes\n"); ++failures; }
    if (packet.cursor != out + 16 + 104) { std::fprintf(stderr, "fail: cursor\n"); ++failures; }
    if (packet.last != out + 8) { std::fprintf(stderr, "fail: last changed\n"); ++failures; }
    for (int i = 0; i < 16; ++i) if (out[i] != 0xa5) { std::fprintf(stderr, "fail: wrote before cursor\n"); ++failures; break; }
    for (int i = 16 + 104; i < 256; ++i) if (out[i] != 0xa5) { std::fprintf(stderr, "fail: wrote past packet\n"); ++failures; break; }
    if (failures) return 1;
    std::printf("lightSetupAmbientOnly contract passed\n");
    return 0;
}
