// Host contract for _GLOBAL_$I$lastkey. Not an EE plan input.
#include <cstdio>
struct VIKeyboard { int word0, word1; };
extern "C" VIKeyboard vik = {7, 7};
static int constructed, destroyed, last_flags;
static VIKeyboard* seen;
extern "C" void __10VIKeyboard(VIKeyboard* self) { ++constructed; seen = self; }
extern "C" void _$_10VIKeyboard(VIKeyboard* self, int flags) { ++destroyed; seen = self; last_flags = flags; }
extern "C" void _GLOBAL_$I$lastkey();
int main() {
    int failures = 0;
    _GLOBAL_$I$lastkey();
    if (constructed != 1 || destroyed != 0 || seen != &vik) { std::fprintf(stderr, "fail: constructs vik once, no destroy\n"); ++failures; }
    if (failures) return 1;
    std::printf("_GLOBAL_$I$lastkey contract passed\n");
    return 0;
}
