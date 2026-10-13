// Host check for groundType. Not an EE plan input.
#include <cstdio>
#include "ground_type_data.cpp"
int main() {
    if (groundType != 0 || sizeof groundType != 4) return 1;
    std::puts("groundType check passed");
    return 0;
}
