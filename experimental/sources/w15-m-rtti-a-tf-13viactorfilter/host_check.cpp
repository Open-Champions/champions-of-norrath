// Host contract for __tf13VIActorFilter; not an EE input. Callees record their arguments; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti13VIActorFilter[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti13VIActorFilter ? "13VIActorFilter" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "user(13VIActorFilter,13VIActorFilter);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti13VIActorFilter, 0x5a, sizeof __ti13VIActorFilter);
        if (r.set) std::memcpy(__ti13VIActorFilter, set_words[pat], 4);
        else std::memset(__ti13VIActorFilter, 0, 4);
        calls.clear();
        void* got = __tf13VIActorFilter();
        if (got != __ti13VIActorFilter) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w15-m-rtti-a-tf-13viactorfilter ok");
    return failures ? 1 : 0;
}
