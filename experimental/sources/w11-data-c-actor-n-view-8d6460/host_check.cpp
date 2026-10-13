// Host contract for norrath_w10_actor_n_view_8d6460. Not an EE plan input.
extern "C" unsigned char norrath_w10_actor_n_view_8d6460[80];
int main() {
    if (sizeof norrath_w10_actor_n_view_8d6460 != 80u) return 1;
    for (unsigned i = 0; i < sizeof norrath_w10_actor_n_view_8d6460; ++i)
        if (norrath_w10_actor_n_view_8d6460[i] != 0) return 2;
    return 0;
}
