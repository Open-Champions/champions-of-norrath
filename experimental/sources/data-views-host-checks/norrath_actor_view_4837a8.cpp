// Host comparison only. Not an EE build input.
extern "C" const char norrath_actor_view_4837a8[];
int main() {
    const char* expect = "wait";
    for (unsigned i = 0;; ++i) {
        if (norrath_actor_view_4837a8[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
