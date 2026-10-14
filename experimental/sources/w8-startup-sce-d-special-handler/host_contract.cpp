#include "special_handler.cpp"

#include <cstdio>

static void sample(int, const unsigned char*) {}

int main() {
    static_assert(sizeof(specialHandler) == sizeof(void*), "one function pointer");
    if (specialHandler != nullptr) return 1;
    specialHandler = sample;
    if (specialHandler != sample) return 2;
    std::puts("special handler contract passed");
    return 0;
}
