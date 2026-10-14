// Host contract for explode_arrow_msg_run_particle_8d7090. Not an EE plan input.
extern "C" unsigned char explode_arrow_msg_run_particle_8d7090[80];
int main() {
    if (sizeof explode_arrow_msg_run_particle_8d7090 != 80u) return 1;
    for (unsigned i = 0; i < sizeof explode_arrow_msg_run_particle_8d7090; ++i)
        if (explode_arrow_msg_run_particle_8d7090[i] != 0) return 2;
    return 0;
}
