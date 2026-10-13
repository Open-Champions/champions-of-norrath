// Host contract for ballSparkParticle. Not an EE plan input.
extern "C" unsigned char ballSparkParticle[80];
int main() {
    if (sizeof ballSparkParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof ballSparkParticle; ++i)
        if (ballSparkParticle[i] != 0) return 2;
    return 0;
}
