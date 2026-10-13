// Host comparison only. Not an EE build input.
extern "C" const char norrath_ctors_lava_wraith_view_485330[];
int main() {
    const char* expect = "fx.lmp";
    for (unsigned i = 0;; ++i) {
        if (norrath_ctors_lava_wraith_view_485330[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
