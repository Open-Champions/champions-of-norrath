// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_actor_view_42baa8[12];
int main() {
    {
        const char* got = norrath_actor_view_42baa8[0].name;
        const char* expect = "g_naviga.lmp";
        if (norrath_actor_view_42baa8[0].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[1].name;
        const char* expect = "navigator.vif";
        if (norrath_actor_view_42baa8[1].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[2].name;
        const char* expect = "navigator.tex";
        if (norrath_actor_view_42baa8[2].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[3].name;
        const char* expect = "idle1.anm";
        if (norrath_actor_view_42baa8[3].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[4].name;
        const char* expect = "idle2.anm";
        if (norrath_actor_view_42baa8[4].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[5].name;
        const char* expect = "idle3.anm";
        if (norrath_actor_view_42baa8[5].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[6].name;
        const char* expect = "run.anm";
        if (norrath_actor_view_42baa8[6].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[7].name;
        const char* expect = "talk1.anm";
        if (norrath_actor_view_42baa8[7].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[8].name;
        const char* expect = "talk2.anm";
        if (norrath_actor_view_42baa8[8].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[9].name;
        const char* expect = "talk3.anm";
        if (norrath_actor_view_42baa8[9].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42baa8[10].name;
        const char* expect = "walk.anm";
        if (norrath_actor_view_42baa8[10].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    if (norrath_actor_view_42baa8[11].name != nullptr || norrath_actor_view_42baa8[11].unused != 0u) return 1;
    return 0;
}
