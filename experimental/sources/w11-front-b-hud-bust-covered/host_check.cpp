// Host check for hudBustCovered. Not an EE plan input.
#include "hud_bust_covered.cpp"
int main() {
    static_assert(sizeof(hudBustCovered) == 1, "size");
    return hudBustCovered == 0 ? 0 : 1;
}
