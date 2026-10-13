// Host comparison only. Not an EE build input.
extern "C" const char norrath_ctors_lava_wraith_view_484fa8[];
int main() {
    const char* expect = "laddup";
    for (unsigned i = 0;; ++i) {
        if (norrath_ctors_lava_wraith_view_484fa8[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
