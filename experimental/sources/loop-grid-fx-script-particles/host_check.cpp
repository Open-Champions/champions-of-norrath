// Host contract for scriptParticles. Not an EE plan input.
#include <cstdio>

extern "C" {
extern unsigned char scriptParticle0[80];
extern unsigned char scriptParticle1[80];
extern unsigned char scriptParticle2[80];
extern unsigned char* scriptParticles[4];
}

int main() {
    if (scriptParticles[0] != scriptParticle0 || scriptParticles[1] != scriptParticle1 || scriptParticles[2] != scriptParticle2) {
        std::fprintf(stderr, "fail: table does not point at the three definitions\n");
        return 1;
    }
    if (scriptParticles[3] != nullptr) { std::fprintf(stderr, "fail: terminator\n"); return 2; }
    for (int n = 0; n < 80; ++n)
        if (scriptParticle0[n] != 0 || scriptParticle1[n] != 0 || scriptParticle2[n] != 0) { std::fprintf(stderr, "fail: nonzero start\n"); return 4; }
    std::printf("script-particles contract passed\n");
    return 0;
}
