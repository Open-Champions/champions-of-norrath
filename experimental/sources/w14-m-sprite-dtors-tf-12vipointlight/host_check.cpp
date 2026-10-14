// Host contract for __tf12VIPointLight; not an EE input. Callees record their arguments; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8VISprite[12];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti12VIPointLight) return "12VIPointLight";
    if (p == __ti8VISprite) return "8VISprite";
    return "?";
}
extern "C" void* __tf8VISprite() { calls += "__tf8VISprite;"; return __ti8VISprite; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + node_name(t) + "," + node_name(base) + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "__tf8VISprite;si(12VIPointLight,8VISprite,12VIPointLight);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti12VIPointLight, 0x5a, sizeof __ti12VIPointLight);
        if (r.set) std::memcpy(__ti12VIPointLight, set_words[pat], 4);
        else std::memset(__ti12VIPointLight, 0, 4);
        calls.clear();
        void* got = __tf12VIPointLight();
        if (got != __ti12VIPointLight) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w14-m-sprite-dtors-tf-12vipointlight ok");
    return failures ? 1 : 0;
}
