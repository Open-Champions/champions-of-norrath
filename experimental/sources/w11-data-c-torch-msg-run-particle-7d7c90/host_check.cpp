// Host contract for torch_msg_run_particle_7d7c90. Not an EE plan input.
extern "C" unsigned char torch_msg_run_particle_7d7c90[80];
int main() {
    if (sizeof torch_msg_run_particle_7d7c90 != 80u) return 1;
    for (unsigned i = 0; i < sizeof torch_msg_run_particle_7d7c90; ++i)
        if (torch_msg_run_particle_7d7c90[i] != 0) return 2;
    return 0;
}
