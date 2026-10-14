// Host contract for waveMainParticle. Not an EE plan input.
extern "C" unsigned char waveMainParticle[80];
int main() {
    if (sizeof waveMainParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof waveMainParticle; ++i)
        if (waveMainParticle[i] != 0) return 2;
    return 0;
}
