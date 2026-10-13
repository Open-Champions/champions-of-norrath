// Host check for the throbB object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_throb_b.cpp"

int main() {
    if (sizeof(throbB) != 4) return 1;
    if (throbB != 0) return 2;
    return 0;
}
