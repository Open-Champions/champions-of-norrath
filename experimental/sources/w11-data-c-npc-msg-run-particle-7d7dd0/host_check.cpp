// Host contract for npc_msg_run_particle_7d7dd0. Not an EE plan input.
extern "C" unsigned char npc_msg_run_particle_7d7dd0[80];
int main() {
    if (sizeof npc_msg_run_particle_7d7dd0 != 80u) return 1;
    for (unsigned i = 0; i < sizeof npc_msg_run_particle_7d7dd0; ++i)
        if (npc_msg_run_particle_7d7dd0[i] != 0) return 2;
    return 0;
}
