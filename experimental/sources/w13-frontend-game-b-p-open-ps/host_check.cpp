// Host contract for P_OpenParticleSystem__Fi. Not an EE plan input.
#include <cstdio>
extern "C" {
unsigned char PS[16 * 312 + 312 * 240];
unsigned char* currentPS;
void P_OpenParticleSystem__Fi(int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    unsigned char* sentinel = PS + 5;
    *reinterpret_cast<int*>(PS + 3 * 312 + 304) = 3;
    *reinterpret_cast<int*>(PS + 3 * 312 + 304) = 3;
    currentPS = sentinel; P_OpenParticleSystem__Fi(0);
    expect(currentPS == sentinel, "zero id changes nothing");
    P_OpenParticleSystem__Fi(3);
    expect(currentPS == PS + 3 * 312, "matching id selects the record");
    currentPS = sentinel; P_OpenParticleSystem__Fi(3 + 256);
    expect(currentPS == sentinel, "same low byte, different id is refused");
    *reinterpret_cast<int*>(PS + 3 * 312 + 304) = 3 + 256;
    P_OpenParticleSystem__Fi(3 + 256);
    expect(currentPS == PS + 3 * 312, "full id equal selects the low-byte record");
    if (failures) return 1;
    std::printf("P_OpenParticleSystem contract passed\n");
    return 0;
}
