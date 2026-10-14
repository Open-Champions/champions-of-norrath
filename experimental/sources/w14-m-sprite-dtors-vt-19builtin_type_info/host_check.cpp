// Host contract for _vt$19__builtin_type_info; not an EE input. Checks the slot layout and the null last slot only.
#include <cstdio>
#include <cstddef>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
static void f0() {}
static VtSlot vt_19__builtin_type_info[3] = {
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, nullptr},
};
int main() {
    int failures = 0;
    if (sizeof vt_19__builtin_type_info / sizeof vt_19__builtin_type_info[0] != 3) { std::fprintf(stderr, "fail: count\n"); ++failures; }
    for (int i = 0; i < 2; ++i) if (vt_19__builtin_type_info[i].adjustment != 0u || vt_19__builtin_type_info[i].function == nullptr) { std::fprintf(stderr, "fail: slot %d\n", i); ++failures; }
    if (vt_19__builtin_type_info[2].function != nullptr || vt_19__builtin_type_info[2].adjustment != 0u) { std::fprintf(stderr, "fail: last\n"); ++failures; }
    if (!failures) std::puts("w14-m-sprite-dtors-vt-19builtin_type_info ok");
    return failures ? 1 : 0;
}
