// Host comparison only. Not an EE build input.
extern "C" const char norrath_ctors_lava_wraith_view_46cd30[];
int main() {
    const char expected[] = "ladddown";
    for (unsigned i = 0; i < sizeof expected; ++i) {
        if (norrath_ctors_lava_wraith_view_46cd30[i] != expected[i]) return 1;
    }
    return 0;
}
