// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_check.cpp
#include <climits>
#include <cstdio>
#include "min.cpp"
int main() {
    const int values[] = {INT_MIN, -70000, -2, -1, 0, 1, 2, 65535, 70000, INT_MAX};
    for (int a : values)
        for (int b : values) {
            const int expected = (a < b) ? a : b;
            if (min(a, b) != expected) { std::printf("min(%d, %d) wrong\n", a, b); return 1; }
        }
    std::printf("min cases passed\n");
    return 0;
}
