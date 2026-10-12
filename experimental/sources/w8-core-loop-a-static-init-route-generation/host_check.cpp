// Host check for _GLOBAL_$I$route_generation. Not an EE plan input.
#include "static_init_route_generation.cpp"

int main() {
    // The original has no memory effect, so the check calls it repeatedly and confirms control returns.
    _GLOBAL_$I$route_generation();
    _GLOBAL_$I$route_generation();
    return 0;
}
