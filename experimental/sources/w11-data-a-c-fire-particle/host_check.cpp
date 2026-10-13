// Host contract for cFireParticle. Not an EE plan input.
extern "C" unsigned char cFireParticle[80];
int main() {
    if (sizeof cFireParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof cFireParticle; ++i)
        if (cFireParticle[i] != 0) return 2;
    return 0;
}
