// Host contract for ectopileParticle1. Not an EE plan input.
extern "C" unsigned char ectopileParticle1[80];
int main() {
    if (sizeof ectopileParticle1 != 80u) return 1;
    for (unsigned i = 0; i < sizeof ectopileParticle1; ++i)
        if (ectopileParticle1[i] != 0) return 2;
    return 0;
}
