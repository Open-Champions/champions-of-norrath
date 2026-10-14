// Host comparison only. Not an EE build input.
extern "C" unsigned char norrath_ctors_lava_wraith_view_601030[4000];
int main() {
    for (unsigned i = 0; i < 4000u; ++i) if (norrath_ctors_lava_wraith_view_601030[i] != 0) return 1;
    return sizeof(norrath_ctors_lava_wraith_view_601030) == 4000u ? 0 : 1;
}
