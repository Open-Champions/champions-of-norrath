// Host contract for ballSmokeParticle. Not an EE plan input.
extern "C" unsigned char ballSmokeParticle[80];
int main() {
    if (sizeof ballSmokeParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof ballSmokeParticle; ++i)
        if (ballSmokeParticle[i] != 0) return 2;
    return 0;
}
