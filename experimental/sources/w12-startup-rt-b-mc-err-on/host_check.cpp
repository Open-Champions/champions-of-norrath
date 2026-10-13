// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <cstdio>
#include "mc_err_on.cpp"
int main() {
    if (sizeof mcErrOn != 4) { std::printf("mcErrOn must be four bytes\n"); return 1; }
    if (mcErrOn != 0) { std::printf("mcErrOn must start at zero\n"); return 2; }
    std::printf("mcErrOn cases passed\n");
    return 0;
}
