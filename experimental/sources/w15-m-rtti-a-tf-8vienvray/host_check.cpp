// Host contract for __tf8VIEnvRay; not an EE input. Callees record their arguments; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8VIEnvRay[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void __rtti_user(void* t, const char* name) {
    calls += std::string("user(") + (t == __ti8VIEnvRay ? "8VIEnvRay" : "?") + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "user(8VIEnvRay,8VIEnvRay);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti8VIEnvRay, 0x5a, sizeof __ti8VIEnvRay);
        if (r.set) std::memcpy(__ti8VIEnvRay, set_words[pat], 4);
        else std::memset(__ti8VIEnvRay, 0, 4);
        calls.clear();
        void* got = __tf8VIEnvRay();
        if (got != __ti8VIEnvRay) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w15-m-rtti-a-tf-8vienvray ok");
    return failures ? 1 : 0;
}
