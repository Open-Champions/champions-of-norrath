// Host contract for modelSetNoZWrite__Fv. Not an EE plan input.
#include <cstdio>
extern "C" {
int modelOption;
void modelSetNoZWrite__Fv();
}
int main() {
    int failures = 0;
    const int starts[] = {0, 1, 2, 6, -1, 0x80000000u != 0 ? -2147483647 - 1 : 0};
    for (int s : starts) {
        modelOption = s;
        modelSetNoZWrite__Fv();
        if (modelOption != (s | 1)) { std::fprintf(stderr, "fail: start %d got %d\n", s, modelOption); ++failures; }
    }
    if (failures) return 1;
    std::printf("modelSetNoZWrite contract passed\n");
    return 0;
}
