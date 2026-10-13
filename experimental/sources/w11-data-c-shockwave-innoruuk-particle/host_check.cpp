// Host contract for shockwaveInnoruukParticle. Not an EE plan input.
extern "C" unsigned char shockwaveInnoruukParticle[80];
int main() {
    if (sizeof shockwaveInnoruukParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof shockwaveInnoruukParticle; ++i)
        if (shockwaveInnoruukParticle[i] != 0) return 2;
    return 0;
}
