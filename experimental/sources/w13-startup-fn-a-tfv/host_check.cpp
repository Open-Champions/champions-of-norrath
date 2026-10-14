// Host check only; not an EE build input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
#include <cstdio>
#include <cstring>
extern "C" { unsigned char _vt$19__builtin_type_info[24]; }
#include "typeinfo.cpp"
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::fprintf(stderr, "fail: %s\n", what); ++failures; } }
static const void* word(int i) { return reinterpret_cast<const void* const*>(__tiv)[i]; }
int main() {
    std::memset(__tiv, 0, sizeof __tiv);
    expect(__tfv() == __tiv, "returns the node");
    expect(word(0) && std::strcmp(static_cast<const char*>(word(0)), "v") == 0, "word 0 is the name text v");
    expect(word(1) == _vt$19__builtin_type_info, "word 1 is the builtin vtable");
    const void* name = word(0);
    expect(__tfv() == __tiv && word(0) == name, "second call leaves the node alone");
    for (int byte = 0; byte < (int)sizeof(void*); ++byte) {
        std::memset(__tiv, 0, sizeof __tiv); __tiv[byte] = 1;
        expect(__tfv() == __tiv && word(1) == nullptr, "any nonzero byte of the first word skips");
    }
    std::memset(__tiv, 0, sizeof __tiv); reinterpret_cast<void**>(__tiv)[1] = _vt$19__builtin_type_info;
    __tfv();
    expect(word(0) != nullptr, "only the first word gates initialization");
    if (!failures) std::puts("tfv ok");
    return failures ? 1 : 0;
}
