#include "dprint.cpp"

#include <cstdio>

int main() {
    DPRINT("libmtap: bind failed\n");
    DPRINT("value %d %s", 3, "x");
    std::puts("dprint contract passed");
    return 0;
}
