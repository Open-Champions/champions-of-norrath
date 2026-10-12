// Host contract for _GLOBAL_$D$lastkey. Not an EE plan input.
#include <cstdio>
struct VIKeyboard { int word0, word1; };
extern "C" VIKeyboard vik = {7, 7};
static int constructed, destroyed, last_flags;
static VIKeyboard* seen;
extern "C" void __10VIKeyboard(VIKeyboard* self) { ++constructed; seen = self; }
extern "C" void _$_10VIKeyboard(VIKeyboard* self, int flags) { ++destroyed; seen = self; last_flags = flags; }
extern "C" void _GLOBAL_$D$lastkey();
int main() {
    int failures = 0;
    _GLOBAL_$D$lastkey();
    if (constructed != 0 || destroyed != 1 || seen != &vik || last_flags != 2) { std::fprintf(stderr, "fail: destroys vik once with flag 2\n"); ++failures; }
    if (failures) return 1;
    std::printf("_GLOBAL_$D$lastkey contract passed\n");
    return 0;
}
