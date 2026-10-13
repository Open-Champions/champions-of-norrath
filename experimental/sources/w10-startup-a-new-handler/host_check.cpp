// Host comparison only. Not an EE build input.
#include <cstdio>
#include "new_handler.cpp"
static void handler() {}
int main() {
    if (__new_handler != nullptr) { std::puts("not null at start"); return 1; }
    __new_handler = handler;
    if (__new_handler != handler) { std::puts("not writable"); return 2; }
    __new_handler = nullptr;
    std::puts("new_handler cases passed");
    return 0;
}
