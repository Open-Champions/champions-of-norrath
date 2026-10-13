// Host contract for ballParticle. Not an EE plan input.
extern "C" unsigned char ballParticle[80];
int main() {
    if (sizeof ballParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof ballParticle; ++i)
        if (ballParticle[i] != 0) return 2;
    return 0;
}
