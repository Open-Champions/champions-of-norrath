// Host contract for lightClear__Fv. Not an EE plan input. The callee must see
// the zero already stored, so it records the word it finds.
#include <cstdio>

extern "C" void lightClear__Fv();
extern "C" int interfaceMode;
int interfaceMode = 9;
static int calls = 0;
static float seen = 0.0f;
static int mode_at_call = -1;
extern "C" void lightGlobalCharLightScale__Ff(float scale) {
    ++calls;
    seen = scale;
    mode_at_call = interfaceMode;
}

int main() {
    lightClear__Fv();
    if (calls != 1) { std::fprintf(stderr, "fail: call count %d\n", calls); return 1; }
    if (seen != 1.0f) { std::fprintf(stderr, "fail: scale %f\n", seen); return 1; }
    if (interfaceMode != 0) { std::fprintf(stderr, "fail: interfaceMode %d\n", interfaceMode); return 1; }
    if (mode_at_call != 0) { std::fprintf(stderr, "fail: store came after the call\n"); return 1; }
    return 0;
}
