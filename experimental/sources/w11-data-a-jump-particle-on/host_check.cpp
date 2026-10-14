// Host contract for jumpParticleOn. Not an EE plan input.
extern "C" unsigned char jumpParticleOn[80];
int main() {
    if (sizeof jumpParticleOn != 80u) return 1;
    for (unsigned i = 0; i < sizeof jumpParticleOn; ++i)
        if (jumpParticleOn[i] != 0) return 2;
    return 0;
}
