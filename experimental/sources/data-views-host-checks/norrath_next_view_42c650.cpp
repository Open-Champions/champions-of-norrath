// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_next_view_42c650[4];
int main() {
    {
        const char* got = norrath_next_view_42c650[0].name;
        const char* expect = "lever.lmp";
        if (norrath_next_view_42c650[0].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_next_view_42c650[1].name;
        const char* expect = "lever.vif";
        if (norrath_next_view_42c650[1].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_next_view_42c650[2].name;
        const char* expect = "lever.tex";
        if (norrath_next_view_42c650[2].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    if (norrath_next_view_42c650[3].name != nullptr || norrath_next_view_42c650[3].unused != 0u) return 1;
    return 0;
}
