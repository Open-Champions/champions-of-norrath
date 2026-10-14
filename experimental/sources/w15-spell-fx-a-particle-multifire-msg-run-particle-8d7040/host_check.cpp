// Host contract for multifire_msg_run_particle_8d7040. Not an EE plan input.
extern "C" unsigned char multifire_msg_run_particle_8d7040[80];
int main() {
    if (sizeof multifire_msg_run_particle_8d7040 != 80u) return 1;
    for (unsigned i = 0; i < sizeof multifire_msg_run_particle_8d7040; ++i)
        if (multifire_msg_run_particle_8d7040[i] != 0) return 2;
    return 0;
}
