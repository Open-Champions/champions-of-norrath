// Host contract for fxClear__Fv. Not an EE plan input.
#include <cstdio>

extern "C" void fxClear__Fv();
static int calls = 0;
static int seen[8];
extern "C" void fxUpdateDust__Fiiiiiiii(int a, int b, int c, int d, int e, int f, int g, int h) {
    const int v[8] = {a, b, c, d, e, f, g, h};
    for (int i = 0; i < 8; ++i) seen[i] = v[i];
    ++calls;
}

int main() {
    fxClear__Fv();
    const int want[8] = {92, 90, 64, 34, 92, 85, 64, 0};
    if (calls != 1) { std::fprintf(stderr, "fail: call count %d\n", calls); return 1; }
    for (int i = 0; i < 8; ++i) {
        if (seen[i] != want[i]) { std::fprintf(stderr, "fail: argument %d is %d\n", i, seen[i]); return 1; }
    }
    return 0;
}
