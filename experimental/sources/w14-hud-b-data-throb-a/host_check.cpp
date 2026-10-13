// Host check for the throbA object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_throb_a.cpp"

int main() {
    if (sizeof(throbA) != 4) return 1;
    if (throbA != 0) return 2;
    return 0;
}
