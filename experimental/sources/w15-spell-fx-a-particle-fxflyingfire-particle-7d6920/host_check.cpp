// Host contract for fxflyingfire_particle_7d6920. Not an EE plan input.
extern "C" unsigned char fxflyingfire_particle_7d6920[80];
int main() {
    if (sizeof fxflyingfire_particle_7d6920 != 80u) return 1;
    for (unsigned i = 0; i < sizeof fxflyingfire_particle_7d6920; ++i)
        if (fxflyingfire_particle_7d6920[i] != 0) return 2;
    return 0;
}
