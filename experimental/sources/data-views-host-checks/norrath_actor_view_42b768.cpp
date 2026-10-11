// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_actor_view_42b768[16];
int main() {
    {
        const char* got = norrath_actor_view_42b768[0].name;
        const char* expect = "mermaid.lmp";
        if (norrath_actor_view_42b768[0].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[1].name;
        const char* expect = "mermaid.vif";
        if (norrath_actor_view_42b768[1].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[2].name;
        const char* expect = "mermaid.tex";
        if (norrath_actor_view_42b768[2].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[3].name;
        const char* expect = "idlerok1.anm";
        if (norrath_actor_view_42b768[3].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[4].name;
        const char* expect = "idlerok2.anm";
        if (norrath_actor_view_42b768[4].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[5].name;
        const char* expect = "idlrok1d2.anm";
        if (norrath_actor_view_42b768[5].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[6].name;
        const char* expect = "rockflip.anm";
        if (norrath_actor_view_42b768[6].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[7].name;
        const char* expect = "sunbath1.anm";
        if (norrath_actor_view_42b768[7].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[8].name;
        const char* expect = "sunbath1d2.anm";
        if (norrath_actor_view_42b768[8].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[9].name;
        const char* expect = "sunbath2.anm";
        if (norrath_actor_view_42b768[9].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[10].name;
        const char* expect = "swim.anm";
        if (norrath_actor_view_42b768[10].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[11].name;
        const char* expect = "swimidle.anm";
        if (norrath_actor_view_42b768[11].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[12].name;
        const char* expect = "swimleft.anm";
        if (norrath_actor_view_42b768[12].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[13].name;
        const char* expect = "swimright.anm";
        if (norrath_actor_view_42b768[13].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42b768[14].name;
        const char* expect = "swimstop.anm";
        if (norrath_actor_view_42b768[14].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    if (norrath_actor_view_42b768[15].name != nullptr || norrath_actor_view_42b768[15].unused != 0u) return 1;
    return 0;
}
