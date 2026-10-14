// Host check only; not an EE build input. Checks the observed extent (24 bytes) and the zero initial value.
#include <cstdio>
extern "C" int instmap_bounds[6];
int main() {
    if (sizeof(instmap_bounds) != 24) return 1;
    for (int v : instmap_bounds) if (v != 0) return 2;
    std::puts("instmap_bounds ok");
    return 0;
}
