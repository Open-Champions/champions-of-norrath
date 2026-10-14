// Host check for nm_direct_qwords.cpp; not an EE plan input.
#include "nm_direct_qwords.cpp"

int main() {
    if (nmDirectQWords != 0) return 1;
    nmDirectQWords = 6;
    if (nmDirectQWords != 6) return 2;
    return 0;
}
