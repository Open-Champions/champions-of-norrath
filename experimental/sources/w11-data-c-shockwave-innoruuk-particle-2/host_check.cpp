// Host contract for shockwaveInnoruukParticle2. Not an EE plan input.
extern "C" unsigned char shockwaveInnoruukParticle2[80];
int main() {
    if (sizeof shockwaveInnoruukParticle2 != 80u) return 1;
    for (unsigned i = 0; i < sizeof shockwaveInnoruukParticle2; ++i)
        if (shockwaveInnoruukParticle2[i] != 0) return 2;
    return 0;
}
