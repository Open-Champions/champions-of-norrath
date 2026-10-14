// Host check, not a plan input.
#include <cstdio>
#include "hostname.cpp"
int main() {
    if (sizeof hostname != 128) return 1;
    for (char c : hostname) if (c) return 2;
    std::puts("hostname check passed");
    return 0;
}
