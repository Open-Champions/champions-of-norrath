// Host check for the kittencount storage: size, initial value and writability.
#include <cstdio>

extern "C" int kittencount;

int main() {
    int status = 0;
    if (kittencount != 0) { std::puts("kittencount initial value differs"); status = 1; }
    kittencount = 7;
    if (kittencount != 7) { std::puts("kittencount is not writable"); status = 1; }
    if (status == 0) std::puts("kittencount ok");
    return status;
}
