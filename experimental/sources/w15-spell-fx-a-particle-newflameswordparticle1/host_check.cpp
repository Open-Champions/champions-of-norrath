// Host contract for newFlameSwordParticle1. Not an EE plan input.
extern "C" unsigned char newFlameSwordParticle1[80];
int main() {
    if (sizeof newFlameSwordParticle1 != 80u) return 1;
    for (unsigned i = 0; i < sizeof newFlameSwordParticle1; ++i)
        if (newFlameSwordParticle1[i] != 0) return 2;
    return 0;
}
