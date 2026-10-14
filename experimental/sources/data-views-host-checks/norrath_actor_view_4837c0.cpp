// Host comparison only. Not an EE build input.
extern "C" const char norrath_actor_view_4837c0[];
int main() {
    const char* expect = "Scale";
    for (unsigned i = 0;; ++i) {
        if (norrath_actor_view_4837c0[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
