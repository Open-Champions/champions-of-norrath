#include "stack_15.cpp"
int main() {
    static_assert(sizeof(stack_15) == 1024, "1024 bytes");
    if (reinterpret_cast<unsigned long>(stack_15) % 64 != 0) return 1;
    for (unsigned char b : stack_15) if (b != 0) return 2;
    return 0;
}
