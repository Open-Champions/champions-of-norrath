// Host contract for summonhammerParticle. Not an EE plan input.
extern "C" unsigned char summonhammerParticle[80];
int main() {
    if (sizeof summonhammerParticle != 80u) return 1;
    for (unsigned i = 0; i < sizeof summonhammerParticle; ++i)
        if (summonhammerParticle[i] != 0) return 2;
    return 0;
}
