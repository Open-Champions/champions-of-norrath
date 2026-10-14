// Host contract for hornomen_run_particle_8d8650. Not an EE plan input.
extern "C" unsigned char hornomen_run_particle_8d8650[80];
int main() {
    if (sizeof hornomen_run_particle_8d8650 != 80u) return 1;
    for (unsigned i = 0; i < sizeof hornomen_run_particle_8d8650; ++i)
        if (hornomen_run_particle_8d8650[i] != 0) return 2;
    return 0;
}
