// Host contract for smokeParticle. Not an EE plan input.
extern "C" unsigned char smokeParticle[80];
int main() {
    if (sizeof smokeParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof smokeParticle; ++i)
        if (smokeParticle[i] != 0) return 2;
    return 0;
}
