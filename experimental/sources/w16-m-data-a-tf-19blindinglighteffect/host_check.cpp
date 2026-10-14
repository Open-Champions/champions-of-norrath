// Host check only; not an EE input. __tf10GameObject and __rtti_si are recording stubs; the nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti19BlindingLightEffect[12] __attribute__((aligned(4)));
unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "base;"; return __ti10GameObject; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + (t == __ti19BlindingLightEffect ? "node" : "?") + "," + name + "," + (base == __ti10GameObject ? "GameObject" : "?") + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}

int main() {
    int failures = 0;
    const char* want = "base;si(node,19BlindingLightEffect,GameObject);";
    // Zero first word: the base getter runs once, __rtti_si registers this node, and the node is returned.
    std::memset(__ti19BlindingLightEffect, 0, sizeof __ti19BlindingLightEffect);
    calls.clear();
    if (__tf19BlindingLightEffect() != __ti19BlindingLightEffect) { std::fprintf(stderr, "fail: zero word return\n"); ++failures; }
    if (calls != want) { std::fprintf(stderr, "fail: zero word calls %s\n", calls.c_str()); ++failures; }
    // Nonzero first word, with other bytes set: returned at once, no calls.
    const unsigned char set_words[3][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}};
    for (const auto& w : set_words) {
        std::memset(__ti19BlindingLightEffect, 0x5a, sizeof __ti19BlindingLightEffect);
        std::memcpy(__ti19BlindingLightEffect, w, 4);
        calls.clear();
        if (__tf19BlindingLightEffect() != __ti19BlindingLightEffect) { std::fprintf(stderr, "fail: set word return\n"); ++failures; }
        if (!calls.empty()) { std::fprintf(stderr, "fail: set word calls %s\n", calls.c_str()); ++failures; }
    }
    if (!failures) std::puts("w16-m-data-a-tf-19blindinglighteffect ok");
    return failures ? 1 : 0;
}
