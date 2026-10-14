// Host contract for modelSetNoZRead__Fv. Not an EE plan input.
#include <cstdio>
extern "C" {
int modelOption;
void modelSetNoZRead__Fv();
}
int main() {
    int failures = 0;
    const int starts[] = {0, 1, 0x20, 0x1f, -1, -2147483647 - 1, 0x55555555};
    for (int s : starts) {
        modelOption = s;
        modelSetNoZRead__Fv();
        if (modelOption != (s | 0x20)) { std::fprintf(stderr, "fail: start %d got %d\n", s, modelOption); ++failures; }
    }
    if (failures) return 1;
    std::printf("modelSetNoZRead contract passed\n");
    return 0;
}
