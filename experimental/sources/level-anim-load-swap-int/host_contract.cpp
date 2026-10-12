#include "swap_int.cpp"
int main() {
    int a = 5, b = -7;
    norrath_swap_int(&a, &b);
    if (a != -7 || b != 5) return 1;
    int same = 0x7fffffff;
    norrath_swap_int(&same, &same);
    if (same != 0x7fffffff) return 2;
    int pair[3] = {1, 2, 3};
    norrath_swap_int(&pair[0], &pair[2]);
    return (pair[0] == 3 && pair[1] == 2 && pair[2] == 1) ? 0 : 3;
}
