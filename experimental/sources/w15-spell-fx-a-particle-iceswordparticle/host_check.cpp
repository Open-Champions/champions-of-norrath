// Host contract for iceSwordParticle. Not an EE plan input.
extern "C" unsigned char iceSwordParticle[80];
int main() {
    if (sizeof iceSwordParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof iceSwordParticle; ++i)
        if (iceSwordParticle[i] != 0) return 2;
    return 0;
}
