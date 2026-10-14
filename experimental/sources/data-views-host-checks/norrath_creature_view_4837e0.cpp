// Host comparison only. Not an EE build input.
extern "C" const char norrath_creature_view_4837e0[];
int main() {
    const char* expect = "dead";
    for (unsigned i = 0;; ++i) {
        if (norrath_creature_view_4837e0[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
