// Host check for the throbG object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_throb_g.cpp"

int main() {
    if (sizeof(throbG) != 4) return 1;
    if (throbG != 0) return 2;
    return 0;
}
