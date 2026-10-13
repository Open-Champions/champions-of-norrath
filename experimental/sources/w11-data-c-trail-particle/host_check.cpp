// Host contract for trailParticle. Not an EE plan input.
extern "C" unsigned char trailParticle[80];
int main() {
    if (sizeof trailParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof trailParticle; ++i)
        if (trailParticle[i] != 0) return 2;
    return 0;
}
