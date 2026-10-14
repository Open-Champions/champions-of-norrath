// Host comparison only. Not an EE build input.
extern "C" unsigned char norrath_actor_view_50bfd0[240];
int main() {
    if (sizeof norrath_actor_view_50bfd0 != 240u) return 1;
    for (unsigned i = 0; i < 240u; ++i) {
        if (norrath_actor_view_50bfd0[i] != 0) return 1;
    }
    // The last list head is index 59, four bytes before the end.
    norrath_actor_view_50bfd0[59 << 2] = 1u;
    if (norrath_actor_view_50bfd0[(59 << 2) + 4 - 1] != 0) return 1;
    if (norrath_actor_view_50bfd0[58 << 2] != 0) return 1;
    return 0;
}
