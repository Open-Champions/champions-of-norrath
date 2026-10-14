// Host contract for __tf14FireStormFlare; not an EE input. Callees record their arguments; the nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti14FireStormFlare[12] __attribute__((aligned(4)));
unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "__tf10GameObject;"; return __ti10GameObject; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + (t == __ti14FireStormFlare ? "14FireStormFlare" : "?") + "," +
             (base == __ti10GameObject ? "10GameObject" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "__tf10GameObject;si(14FireStormFlare,10GameObject,14FireStormFlare);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti14FireStormFlare, 0x5a, sizeof __ti14FireStormFlare);
        if (r.set) std::memcpy(__ti14FireStormFlare, set_words[pat], 4);
        else std::memset(__ti14FireStormFlare, 0, 4);
        calls.clear();
        void* got = __tf14FireStormFlare();
        if (got != __ti14FireStormFlare) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w15-m-rtti-a-tf-14firestormflare ok");
    return failures ? 1 : 0;
}
