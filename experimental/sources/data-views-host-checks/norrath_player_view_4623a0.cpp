// Host comparison only. Not an EE build input.
extern "C" const char norrath_player_view_4623a0[];
int main() {
    const char* expect = "equipmnt.lmp";
    for (unsigned i = 0;; ++i) {
        if (norrath_player_view_4623a0[i] != expect[i]) return 1;
        if (expect[i] == 0) return 0;
    }
}
