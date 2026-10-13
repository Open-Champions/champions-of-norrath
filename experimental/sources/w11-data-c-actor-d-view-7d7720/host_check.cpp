// Host contract for norrath_w10_actor_d_view_7d7720. Not an EE plan input.
extern "C" unsigned char norrath_w10_actor_d_view_7d7720[80];
int main() {
    if (sizeof norrath_w10_actor_d_view_7d7720 != 80u) return 1;
    for (unsigned i = 0; i < sizeof norrath_w10_actor_d_view_7d7720; ++i)
        if (norrath_w10_actor_d_view_7d7720[i] != 0) return 2;
    return 0;
}
