// Host contract for the lightningSwordParticle object. Not an EE plan input.
#include <cstdio>
#include "lightning_sword_particle.cpp"
int main() {
    for (unsigned i = 0; i < sizeof lightningSwordParticle; ++i)
        if (lightningSwordParticle[i] != 0) { std::fprintf(stderr, "fail: byte %u not zero\n", i); return 1; }
    std::printf("lightningSwordParticle contract passed\n");
    return 0;
}
