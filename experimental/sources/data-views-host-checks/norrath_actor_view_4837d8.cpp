// Host comparison only. Not an EE build input.
extern "C" const char norrath_actor_view_4837d8[];
int main() {
    const char* expect = "ambient";
    for (unsigned i = 0;; ++i) {
        if (norrath_actor_view_4837d8[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
