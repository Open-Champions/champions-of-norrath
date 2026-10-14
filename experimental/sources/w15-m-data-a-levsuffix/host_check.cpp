// Host check for the levsuffix storage: size, initial value and writability.
#include <cstdio>

extern "C" int levsuffix;

int main() {
    int status = 0;
    if (levsuffix != 0) { std::puts("levsuffix initial value differs"); status = 1; }
    levsuffix = 7;
    if (levsuffix != 7) { std::puts("levsuffix is not writable"); status = 1; }
    if (status == 0) std::puts("levsuffix ok");
    return status;
}
