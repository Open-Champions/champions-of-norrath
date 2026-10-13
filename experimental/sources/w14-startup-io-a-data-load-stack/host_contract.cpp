#include "load_stack.cpp"
int main() {
    static_assert(sizeof(loadStack) == 8192, "stack size passed to the thread");
    for (unsigned char byte : loadStack) if (byte != 0) return 1;
    return 0;
}
