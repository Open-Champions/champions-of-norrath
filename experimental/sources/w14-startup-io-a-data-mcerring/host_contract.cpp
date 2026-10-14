#include "mcerring.cpp"
int main() {
    static_assert(sizeof(mcerring) == 4, "four-byte word");
    if (mcerring != 0) return 1;
    mcerring = 5;
    return mcerring == 5 ? 0 : 2;
}
