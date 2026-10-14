// Host check for the statAllocation object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_stat_allocation.cpp"

int main() {
    if (sizeof(statAllocation) != 4) return 1;
    if (statAllocation != 0) return 2;
    return 0;
}
