// Host check for p.cpp; not an EE plan input.
#include "p.cpp"

int main() {
    if (p[0] != 0 || p[3] != 0) return 1;
    p[2] = 0xabcd;
    if (p[2] != 0xabcd) return 2;
    return 0;
}
