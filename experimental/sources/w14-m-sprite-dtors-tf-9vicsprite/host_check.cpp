// Host contract for __tf9VICSprite; not an EE input. Callees record their arguments; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti9VIHSprite[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti9VICSprite) return "9VICSprite";
    if (p == __ti9VIHSprite) return "9VIHSprite";
    return "?";
}
extern "C" void* __tf9VIHSprite() { calls += "__tf9VIHSprite;"; return __ti9VIHSprite; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + node_name(t) + "," + node_name(base) + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "__tf9VIHSprite;si(9VICSprite,9VIHSprite,9VICSprite);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti9VICSprite, 0x5a, sizeof __ti9VICSprite);
        if (r.set) std::memcpy(__ti9VICSprite, set_words[pat], 4);
        else std::memset(__ti9VICSprite, 0, 4);
        calls.clear();
        void* got = __tf9VICSprite();
        if (got != __ti9VICSprite) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w14-m-sprite-dtors-tf-9vicsprite ok");
    return failures ? 1 : 0;
}
