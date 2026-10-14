// Host comparison only. Not an EE build input.
extern "C" unsigned char norrath_actor_view_486878[8];
int main() {
    if (sizeof norrath_actor_view_486878 != 8u) return 1;
    for (unsigned i = 0; i < 8u; ++i) {
        if (norrath_actor_view_486878[i] != 0) return 1;
    }
    norrath_actor_view_486878[0] = 1u;
    if (norrath_actor_view_486878[0] != 1u) return 1;
    if (norrath_actor_view_486878[4] != 0) return 1;
    return 0;
}
