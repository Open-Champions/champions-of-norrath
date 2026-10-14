// Host contract for diseaseshield_run_particle_8d69b0. Not an EE plan input.
extern "C" unsigned char diseaseshield_run_particle_8d69b0[80];
int main() {
    if (sizeof diseaseshield_run_particle_8d69b0 != 80u) return 1;
    for (unsigned i = 0; i < sizeof diseaseshield_run_particle_8d69b0; ++i)
        if (diseaseshield_run_particle_8d69b0[i] != 0) return 2;
    return 0;
}
