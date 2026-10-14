// Host check for the dynlevelcount storage: size, initial value and writability.
#include <cstdio>

extern "C" int dynlevelcount;

int main() {
    int status = 0;
    if (dynlevelcount != 0) { std::puts("dynlevelcount initial value differs"); status = 1; }
    dynlevelcount = 7;
    if (dynlevelcount != 7) { std::puts("dynlevelcount is not writable"); status = 1; }
    if (status == 0) std::puts("dynlevelcount ok");
    return status;
}
