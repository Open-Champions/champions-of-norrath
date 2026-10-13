// Host check for the otherHermiteTime object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_other_hermite_time.cpp"

int main() {
    if (sizeof(otherHermiteTime) != 4) return 1;
    if (otherHermiteTime != 0) return 2;
    return 0;
}
