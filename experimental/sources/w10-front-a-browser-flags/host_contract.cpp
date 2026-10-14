#include "browser_flags.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof browserOverwrite == 4 && sizeof browserDelete == 4, "word flags");
    if (browserOverwrite != 0 || browserDelete != 0) return 1;
    std::puts("browser flags passed");
    return 0;
}
