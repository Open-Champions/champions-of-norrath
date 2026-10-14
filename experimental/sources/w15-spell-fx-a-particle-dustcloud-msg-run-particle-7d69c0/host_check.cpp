// Host contract for dustcloud_msg_run_particle_7d69c0. Not an EE plan input.
extern "C" unsigned char dustcloud_msg_run_particle_7d69c0[80];
int main() {
    if (sizeof dustcloud_msg_run_particle_7d69c0 != 80u) return 1;
    for (unsigned i = 0; i < sizeof dustcloud_msg_run_particle_7d69c0; ++i)
        if (dustcloud_msg_run_particle_7d69c0[i] != 0) return 2;
    return 0;
}
