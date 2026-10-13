// Host contract for ant_msg_run_particle_7d7170. Not an EE plan input.
extern "C" unsigned char ant_msg_run_particle_7d7170[80];
int main() {
    for (unsigned i = 0; i < 80; ++i)
        if (ant_msg_run_particle_7d7170[i] != 0) return 1;
    return 0;
}
