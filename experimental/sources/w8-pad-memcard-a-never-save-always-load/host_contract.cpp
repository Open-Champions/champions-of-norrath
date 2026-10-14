#include "never_save_always_load.cpp"

#include <cstdio>

static char a[] = "Lantern", b[] = "Torch", c[] = "zzRoutepoint", hidden[] = "Hidden";
char* neverSave[] = {a, b, c, 0, hidden};
static int calls;
static const char* last_arg;
extern "C" int strcasecmp(const char* x, const char* y) {
    ++calls;
    last_arg = x;
    for (;; ++x, ++y) {
        int l = *x, r = *y;
        if (l >= 'A' && l <= 'Z') l += 32;
        if (r >= 'A' && r <= 'Z') r += 32;
        if (l != r) return l - r;
        if (!l) return 0;
    }
}

int main() {
    char name[] = "LANTERN";
    if (neverSaveAlwaysLoadFromLump__FPc(0) != 0 || calls != 0) return 1;
    if (neverSaveAlwaysLoadFromLump__FPc(name) != 1 || calls != 1 || last_arg != name) return 2;
    char later[] = "zzroutepoint";
    calls = 0;
    if (neverSaveAlwaysLoadFromLump__FPc(later) != 1 || calls != 3) return 3;
    char miss[] = "Hidden";
    calls = 0;
    if (neverSaveAlwaysLoadFromLump__FPc(miss) != 0 || calls != 3) return 4;  // stops at the null entry
    std::puts("never save always load passed");
    return 0;
}
