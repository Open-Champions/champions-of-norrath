// Host check for the statAllocWarn object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_stat_alloc_warn.cpp"

int main() {
    if (sizeof(statAllocWarn) != 1) return 1;
    if (statAllocWarn != 0) return 2;
    return 0;
}
