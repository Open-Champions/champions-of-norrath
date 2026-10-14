// Host comparison only. Not an EE build input.
struct ResourceSlot { char* name; unsigned unused; };
extern "C" ResourceSlot norrath_actor_view_42c130[11];
int main() {
    {
        const char* got = norrath_actor_view_42c130[0].name;
        const char* expect = "blkwidow.lmp";
        if (norrath_actor_view_42c130[0].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[1].name;
        const char* expect = "spider.vif";
        if (norrath_actor_view_42c130[1].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[2].name;
        const char* expect = "spider.tex";
        if (norrath_actor_view_42c130[2].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[3].name;
        const char* expect = "death1.anm";
        if (norrath_actor_view_42c130[3].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[4].name;
        const char* expect = "death2.anm";
        if (norrath_actor_view_42c130[4].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[5].name;
        const char* expect = "death3.anm";
        if (norrath_actor_view_42c130[5].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[6].name;
        const char* expect = "death4.anm";
        if (norrath_actor_view_42c130[6].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[7].name;
        const char* expect = "idle1.anm";
        if (norrath_actor_view_42c130[7].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[8].name;
        const char* expect = "idle2.anm";
        if (norrath_actor_view_42c130[8].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    {
        const char* got = norrath_actor_view_42c130[9].name;
        const char* expect = "walk.anm";
        if (norrath_actor_view_42c130[9].unused != 0u) return 1;
        for (unsigned c = 0;; ++c) {
            if (got[c] != expect[c]) return 1;
            if (expect[c] == 0) break;
        }
    }
    if (norrath_actor_view_42c130[10].name != nullptr || norrath_actor_view_42c130[10].unused != 0u) return 1;
    return 0;
}
