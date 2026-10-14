// Host comparison only. Not an EE build input.
extern "C" const char norrath_player_view_462350[];
int main() {
    const char* expect = "noCollide";
    for (unsigned i = 0;; ++i) {
        if (norrath_player_view_462350[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
