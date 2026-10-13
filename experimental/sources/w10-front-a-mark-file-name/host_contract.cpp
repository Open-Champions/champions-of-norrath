#include "mark_file_name.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof markFileName == 48, "48 bytes");
    for (char c : markFileName) {
        if (c != 0) return 1;
    }
    std::puts("mark file name passed");
    return 0;
}
