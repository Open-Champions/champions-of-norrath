// Host check for scriptClear__Fv. Not an EE plan input.
extern "C" {
int nmDeferedCalls = 3;
int nextNmDeferedCalls = 4;
int scriptFrame = 5;
}
#include "script_clear.cpp"
int main() {
    scriptClear__Fv();
    if (nmDeferedCalls != 0) return 1;
    if (nextNmDeferedCalls != 0) return 2;
    if (scriptFrame != 0) return 3;
    return 0;
}
