// Host contract for __tf8VISprite; not an EE input. Callees record their arguments; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8VISprite[12] __attribute__((aligned(4)));
unsigned char __ti10VIResource[8];
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti8VISprite) return "8VISprite";
    if (p == __ti10VIResource) return "10VIResource";
    return "?";
}
extern "C" void* __tf10VIResource() { calls += "__tf10VIResource;"; return __ti10VIResource; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + node_name(t) + "," + node_name(base) + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "__tf10VIResource;si(8VISprite,10VIResource,8VISprite);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti8VISprite, 0x5a, sizeof __ti8VISprite);
        if (r.set) std::memcpy(__ti8VISprite, set_words[pat], 4);
        else std::memset(__ti8VISprite, 0, 4);
        calls.clear();
        void* got = __tf8VISprite();
        if (got != __ti8VISprite) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w15-m-rtti-a-tf-8visprite ok");
    return failures ? 1 : 0;
}
