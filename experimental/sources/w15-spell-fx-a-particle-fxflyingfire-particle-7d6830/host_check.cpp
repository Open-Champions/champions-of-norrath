// Host contract for fxflyingfire_particle_7d6830. Not an EE plan input.
extern "C" unsigned char fxflyingfire_particle_7d6830[80];
int main() {
    if (sizeof fxflyingfire_particle_7d6830 != 80u) return 1;
    for (unsigned i = 0; i < sizeof fxflyingfire_particle_7d6830; ++i)
        if (fxflyingfire_particle_7d6830[i] != 0) return 2;
    return 0;
}
