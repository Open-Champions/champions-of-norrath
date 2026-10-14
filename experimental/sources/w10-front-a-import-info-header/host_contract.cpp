#include "import_info_header.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof importInfoHeader == 112, "112 bytes");
    for (char c : importInfoHeader) {
        if (c != 0) return 1;
    }
    std::puts("import info header passed");
    return 0;
}
