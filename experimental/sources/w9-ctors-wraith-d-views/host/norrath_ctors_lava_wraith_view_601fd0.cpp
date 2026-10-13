// Host comparison only. Not an EE build input.
extern "C" unsigned norrath_ctors_lava_wraith_view_601fd0[126];
int main() {
    for (unsigned i = 0; i < 126u; ++i) if (norrath_ctors_lava_wraith_view_601fd0[i] != 0u) return 1;
    return sizeof(norrath_ctors_lava_wraith_view_601fd0) == 504u ? 0 : 1;
}
