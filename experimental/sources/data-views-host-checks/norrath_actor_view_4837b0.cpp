// Host comparison only. Not an EE build input.
extern "C" const char norrath_actor_view_4837b0[];
int main() {
    const char* expect = "hide";
    for (unsigned i = 0;; ++i) {
        if (norrath_actor_view_4837b0[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
