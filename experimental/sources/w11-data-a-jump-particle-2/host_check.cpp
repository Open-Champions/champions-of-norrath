// Host contract for jumpParticle2. Not an EE plan input.
extern "C" unsigned char jumpParticle2[80];
int main() {
    if (sizeof jumpParticle2 != 80u) return 1;
    for (unsigned i = 0; i < sizeof jumpParticle2; ++i)
        if (jumpParticle2[i] != 0) return 2;
    return 0;
}
