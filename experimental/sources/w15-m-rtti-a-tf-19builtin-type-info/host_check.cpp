// Host contract for __tf19__builtin_type_info; not an EE input. Callees record their arguments; the node is a local array.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti19__builtin_type_info[12] __attribute__((aligned(4)));
unsigned char __ti9type_info[8] __attribute__((aligned(4)));
static std::string calls;
static const char* node_name(const void* p) {
    if (p == __ti19__builtin_type_info) return "19__builtin_type_info";
    if (p == __ti9type_info) return "9type_info";
    return "?";
}
extern "C" void* __tf9type_info() { calls += "__tf9type_info;"; return __ti9type_info; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + node_name(t) + "," + node_name(base) + "," + name + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}
int main() {
    int failures = 0;
    struct Row { bool set; const char* calls; };
    static const Row rows[] = {
        {false, "__tf9type_info;si(19__builtin_type_info,9type_info,19__builtin_type_info);"},
        {true, ""},
    };
    // A set first word (any nonzero byte pattern) skips all work; a zero word registers. Trailing bytes are not read.
    static const unsigned char set_words[4][4] = {{1, 0, 0, 0}, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0x80}, {2, 0, 0, 0}};
    for (const Row& r : rows) for (int pat = 0; pat < 4; ++pat) {
        std::memset(__ti19__builtin_type_info, 0x5a, sizeof __ti19__builtin_type_info);
        if (r.set) std::memcpy(__ti19__builtin_type_info, set_words[pat], 4);
        else std::memset(__ti19__builtin_type_info, 0, 4);
        calls.clear();
        void* got = __tf19__builtin_type_info();
        if (got != __ti19__builtin_type_info) { std::fprintf(stderr, "fail: wrong node\n"); ++failures; }
        if (calls != r.calls) { std::fprintf(stderr, "fail: got %s want %s\n", calls.c_str(), r.calls); ++failures; }
    }
    if (!failures) std::puts("w15-m-rtti-a-tf-19builtin-type-info ok");
    return failures ? 1 : 0;
}
