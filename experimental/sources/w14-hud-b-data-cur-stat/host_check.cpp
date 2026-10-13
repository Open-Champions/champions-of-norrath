// Host check for the curStat object. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
#include "data_cur_stat.cpp"

int main() {
    if (sizeof(curStat) != 1) return 1;
    if (curStat != 0) return 2;
    return 0;
}
