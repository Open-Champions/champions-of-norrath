// Host contract for vaporParticle. Not an EE plan input.
extern "C" unsigned char vaporParticle[80];
int main() {
    if (sizeof vaporParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof vaporParticle; ++i)
        if (vaporParticle[i] != 0) return 2;
    return 0;
}
