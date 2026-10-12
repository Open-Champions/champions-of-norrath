#include "core_start_loading.cpp"

#include <cstdio>

int drawFrameNm = 77;
int coreLoading = -1;
int coreLoadingStart = -1;

int main() {
    coreStartLoading__Fv();
    if (coreLoading != 10) return 1;
    if (coreLoadingStart != 77) return 2;
    drawFrameNm = 5;
    coreStartLoading__Fv();
    if (coreLoadingStart != 5) return 3;
    std::puts("core start loading passed");
    return 0;
}
