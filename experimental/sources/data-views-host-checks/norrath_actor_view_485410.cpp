// Host comparison only. Not an EE build input.
extern "C" const char norrath_actor_view_485410[];
int main() {
    const char* expect = "pitch";
    for (unsigned i = 0;; ++i) {
        if (norrath_actor_view_485410[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
