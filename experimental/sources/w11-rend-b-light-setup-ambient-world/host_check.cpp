// Host contract for lightSetupAmbientWorld__FP6Packeti. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct Packet { unsigned char* cursor; unsigned char* last; };
extern "C" void lightSetupAmbientWorld__FP6Packeti(Packet*, int);
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char buf[256];
    std::memset(buf, 0xaa, sizeof buf);
    Packet p{buf + 16, nullptr};
    lightSetupAmbientWorld__FP6Packeti(&p, 0x12345);
    auto w = [&](int off) { unsigned v; std::memcpy(&v, buf + 16 + off, 4); return v; };
    auto f = [&](int off) { float v; std::memcpy(&v, buf + 16 + off, 4); return v; };
    auto h = [&](int off) { short v; std::memcpy(&v, buf + 16 + off, 2); return v; };
    expect(p.cursor == buf + 16 + 4 + 140, "cursor advances 144 bytes");
    expect(w(0) == 0x6d048000u, "leading word");
    expect(h(4) == 0x2345 && h(6) == 0x2345 && h(8) == 0x2345 && h(10) == 0, "halfwords from the int");
    for (int i = 12; i < 36; i += 4) expect(w(i) == 0, "zero block base+8..31");
    expect(w(36) == 0x1400000eu && w(40) == 0x6c06c000u, "tag words at base+32 and +36");
    for (int i = 44; i <= 56; i += 4) expect(f(i) == 1.0f, "four ones");
    for (int i = 60; i <= 104; i += 4) expect(w(i) == 0, "zero words base+56..100");
    expect(f(108) == 1.0e21f && f(124) == -1.0f, "1e21 and -1");
    for (int i = 112; i <= 120; i += 4) expect(w(i) == 0, "zero words base+108..116");
    for (int i = 128; i <= 136; i += 4) expect(w(i) == 0, "zero words base+124..132");
    expect(w(140) == 0x14000004u, "closing word");
    expect(buf[15] == 0xaa && buf[16 + 144] == 0xaa, "nothing outside the 144 bytes");
    if (failures) return 1;
    std::printf("lightSetupAmbientWorld contract passed\n");
    return 0;
}
