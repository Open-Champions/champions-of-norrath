// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_actor_view_42d0c0[15];
int main() {
    {
        const char* got = norrath_actor_view_42d0c0[0].name;
        const char* expect = "natasla.lmp";
        if (norrath_actor_view_42d0c0[0].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[1].name;
        const char* expect = "natasla.vif";
        if (norrath_actor_view_42d0c0[1].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[2].name;
        const char* expect = "natasla.tex";
        if (norrath_actor_view_42d0c0[2].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[3].name;
        const char* expect = "idle.anm";
        if (norrath_actor_view_42d0c0[3].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[4].name;
        const char* expect = "parts_1.vif";
        if (norrath_actor_view_42d0c0[4].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[5].name;
        const char* expect = "parts_1.tex";
        if (norrath_actor_view_42d0c0[5].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[6].name;
        const char* expect = "parts_2.vif";
        if (norrath_actor_view_42d0c0[6].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[7].name;
        const char* expect = "parts_2.tex";
        if (norrath_actor_view_42d0c0[7].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[8].name;
        const char* expect = "parts_3.vif";
        if (norrath_actor_view_42d0c0[8].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[9].name;
        const char* expect = "parts_3.tex";
        if (norrath_actor_view_42d0c0[9].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[10].name;
        const char* expect = "parts_4.vif";
        if (norrath_actor_view_42d0c0[10].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[11].name;
        const char* expect = "parts_4.tex";
        if (norrath_actor_view_42d0c0[11].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[12].name;
        const char* expect = "parts_5.vif";
        if (norrath_actor_view_42d0c0[12].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42d0c0[13].name;
        const char* expect = "parts_5.tex";
        if (norrath_actor_view_42d0c0[13].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    if (norrath_actor_view_42d0c0[14].name != nullptr || norrath_actor_view_42d0c0[14].unused != 0u) return 1;
    return 0;
}
