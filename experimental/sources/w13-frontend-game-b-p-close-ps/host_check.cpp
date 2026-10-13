// Host contract for P_CloseParticleSystem__Fi. Not an EE plan input.
#include <cstdio>
extern "C" {
unsigned char PS[312 * 256];
unsigned char globalSystem[256];
unsigned char* currentPS;
void P_CloseParticleSystem__Fi(int);
}
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    unsigned char* sentinel = PS + 7;
    *reinterpret_cast<int*>(PS + 9 * 312 + 304) = 9;
    currentPS = sentinel; P_CloseParticleSystem__Fi(0);
    expect(currentPS == sentinel, "zero id changes nothing");
    P_CloseParticleSystem__Fi(9 + 256);
    expect(currentPS == sentinel, "mismatched id changes nothing");
    P_CloseParticleSystem__Fi(9);
    expect(currentPS == globalSystem, "matching id restores globalSystem");
    if (failures) return 1;
    std::printf("P_CloseParticleSystem contract passed\n");
    return 0;
}
