#include <cstdio>
#include "softdouble_names.cpp"
static unsigned long long seen_left, seen_right; static int last;
extern "C" {
unsigned long long norrath_player_double_add(unsigned long long l, unsigned long long r) { seen_left = l; seen_right = r; last = 1; return l + r + 1; }
unsigned long long norrath_player_double_multiply(unsigned long long l, unsigned long long r) { seen_left = l; seen_right = r; last = 2; return l ^ r; }
int norrath_factory_double_compare(unsigned long long l, unsigned long long r) { seen_left = l; seen_right = r; last = 3; return l < r ? -1 : (l > r ? 1 : 0); }
float norrath_player_double_to_float(unsigned long long v) { seen_left = v; last = 4; return static_cast<float>(v) + 0.5f; }
}
int main() {
    const unsigned long long a = 0x3ff0000000000000ull, b = 0xc000000000000001ull;
    if (norrath_softdouble_add_2fb5c0(a, b) != a + b + 1 || last != 1 || seen_left != a || seen_right != b) return 1;
    if (norrath_softdouble_multiply_2fb680(a, b) != (a ^ b) || last != 2 || seen_left != a || seen_right != b) return 2;
    if (norrath_softdouble_compare_2fba48(a, b) != -1 || last != 3 || seen_left != a || seen_right != b) return 3;
    if (norrath_softdouble_compare_2fba48(b, a) != 1 || norrath_softdouble_compare_2fba48(a, a) != 0) return 4;
    if (norrath_softdouble_to_float_2fbc20(8) != 8.5f || last != 4 || seen_left != 8) return 5;
    std::puts("softdouble names contract passed");
    return 0;
}
