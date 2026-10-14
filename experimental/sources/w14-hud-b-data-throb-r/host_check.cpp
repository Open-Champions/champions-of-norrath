// Host check for the throbR object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_throb_r.cpp"

int main() {
    if (sizeof(throbR) != 4) return 1;
    if (throbR != 0) return 2;
    return 0;
}
