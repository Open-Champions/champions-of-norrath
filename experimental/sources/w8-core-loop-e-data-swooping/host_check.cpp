#include "swooping.cpp"
int main() {
    static_assert(sizeof(swooping) == 4, "one word");
    if (swooping != 0) return 1;
    swooping = 1;
    return swooping == 1 ? 0 : 2;
}
