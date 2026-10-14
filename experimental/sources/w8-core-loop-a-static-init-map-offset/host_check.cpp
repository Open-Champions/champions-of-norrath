// Host check for _GLOBAL_$I$_3map$mapOffset. Not an EE plan input.
#include "static_init_map_offset.cpp"

int main() {
    // The original has no memory effect, so the check calls it repeatedly and confirms control returns.
    _GLOBAL_$I$_3map$mapOffset();
    _GLOBAL_$I$_3map$mapOffset();
    return 0;
}
