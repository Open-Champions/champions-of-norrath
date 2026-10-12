#include "core_loading.cpp"
#include <cstdio>
static_assert(sizeof(coreLoading) == 4, "coreLoading is one word");
int main() {
    if (coreLoading != 0) return 1;
    coreLoading = -2;
    if (coreLoading != -2) return 2;
    std::puts("coreLoading contract passed");
    return 0;
}
