// Host contract for cSmokeParticle. Not an EE plan input.
extern "C" unsigned char cSmokeParticle[80];
int main() {
    if (sizeof cSmokeParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof cSmokeParticle; ++i)
        if (cSmokeParticle[i] != 0) return 2;
    return 0;
}
