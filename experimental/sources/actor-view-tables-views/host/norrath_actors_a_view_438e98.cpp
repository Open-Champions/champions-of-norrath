// Host comparison only. Not an EE build input.
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_actors_a_view_438e98[4];
int main() {
    if (norrath_actors_a_view_438e98[0].unused != 0u) return 1;
    if (!same(norrath_actors_a_view_438e98[0].name, "firebomb.lmp")) return 1;
    if (norrath_actors_a_view_438e98[1].unused != 0u) return 1;
    if (!same(norrath_actors_a_view_438e98[1].name, "firebomb.vif")) return 1;
    if (norrath_actors_a_view_438e98[2].unused != 0u) return 1;
    if (!same(norrath_actors_a_view_438e98[2].name, "firebomb.tex")) return 1;
    if (norrath_actors_a_view_438e98[3].name != nullptr) return 1;
    if (norrath_actors_a_view_438e98[3].unused != 0u) return 1;
    return 0;
}
