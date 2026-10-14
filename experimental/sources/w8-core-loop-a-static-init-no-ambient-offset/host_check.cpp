// Host check for _GLOBAL_$I$noAmbientOffset. Not an EE plan input.
#include "static_init_no_ambient_offset.cpp"

int main() {
    // The original has no memory effect, so the check calls it repeatedly and confirms control returns.
    _GLOBAL_$I$noAmbientOffset();
    _GLOBAL_$I$noAmbientOffset();
    return 0;
}
