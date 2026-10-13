// Host comparison only. Not an EE build input.
extern "C" const char norrath_ctors_lava_wraith_view_474570[];
int main() {
    const char expected[] = "woodelfscout";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_ctors_lava_wraith_view_474570[i] != expected[i]) return 1;
    }
    return 0;
}
