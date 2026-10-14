#include <cstdio>
static unsigned long long seen_left, seen_right;
static int calls;
extern "C" unsigned long long norrath_player_double_multiply(unsigned long long l, unsigned long long r) { ++calls; seen_left = l; seen_right = r; return l * 3 + r; }
#include "dpmul.cpp"
int main() {
    const unsigned long long values[] = {0, 1, 0x8000000000000000ull, 0x3ff0000000000000ull, 0x7ff8000000000000ull, 0xfedcba9876543210ull};
    for (unsigned long long a : values) for (unsigned long long b : values) {
        calls = 0;
        const unsigned long long r = dpmul(a, b);
        if (calls != 1 || seen_left != a || seen_right != b || r != a * 3 + b) { std::puts("dpmul wrong"); return 1; }
    }
    std::puts("dpmul forwards both operands in order and its result");
    return 0;
}
