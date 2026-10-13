// Host contract for innoruukParticle2. Not an EE plan input.
extern "C" unsigned char innoruukParticle2[80];
int main() {
    if (sizeof innoruukParticle2 != 80u) return 1;
    for (unsigned i = 0; i < sizeof innoruukParticle2; ++i)
        if (innoruukParticle2[i] != 0) return 2;
    return 0;
}
