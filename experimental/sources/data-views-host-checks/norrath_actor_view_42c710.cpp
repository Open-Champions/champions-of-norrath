// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_actor_view_42c710[10];
int main() {
    {
        const char* got = norrath_actor_view_42c710[0].name;
        const char* expect = "cdanims.lmp";
        if (norrath_actor_view_42c710[0].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[1].name;
        const char* expect = "bigeye.vif";
        if (norrath_actor_view_42c710[1].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[2].name;
        const char* expect = "bigeye.tex";
        if (norrath_actor_view_42c710[2].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[3].name;
        const char* expect = "biglens.vif";
        if (norrath_actor_view_42c710[3].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[4].name;
        const char* expect = "biglens.tex";
        if (norrath_actor_view_42c710[4].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[5].name;
        const char* expect = "biglid.vif";
        if (norrath_actor_view_42c710[5].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[6].name;
        const char* expect = "biglid.tex";
        if (norrath_actor_view_42c710[6].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[7].name;
        const char* expect = "bigeye.anm";
        if (norrath_actor_view_42c710[7].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c710[8].name;
        const char* expect = "biglid.anm";
        if (norrath_actor_view_42c710[8].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    if (norrath_actor_view_42c710[9].name != nullptr || norrath_actor_view_42c710[9].unused != 0u) return 1;
    return 0;
}
