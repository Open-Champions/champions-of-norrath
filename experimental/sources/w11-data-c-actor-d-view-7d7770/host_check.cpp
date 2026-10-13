// Host contract for norrath_w10_actor_d_view_7d7770. Not an EE plan input.
extern "C" unsigned char norrath_w10_actor_d_view_7d7770[80];
int main() {
    if (sizeof norrath_w10_actor_d_view_7d7770 != 80u) return 1;
    for (unsigned i = 0; i < sizeof norrath_w10_actor_d_view_7d7770; ++i)
        if (norrath_w10_actor_d_view_7d7770[i] != 0) return 2;
    return 0;
}
