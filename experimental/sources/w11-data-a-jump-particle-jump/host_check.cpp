// Host contract for jumpParticleJump. Not an EE plan input.
extern "C" unsigned char jumpParticleJump[80];
int main() {
    if (sizeof jumpParticleJump != 80u) return 1;
    for (unsigned i = 0; i < sizeof jumpParticleJump; ++i)
        if (jumpParticleJump[i] != 0) return 2;
    return 0;
}
