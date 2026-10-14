// Host contract for holyshield_particle_8d7720. Not an EE plan input.
extern "C" unsigned char holyshield_particle_8d7720[80];
int main() {
    if (sizeof holyshield_particle_8d7720 != 80u) return 1;
    for (unsigned i = 0; i < sizeof holyshield_particle_8d7720; ++i)
        if (holyshield_particle_8d7720[i] != 0) return 2;
    return 0;
}
