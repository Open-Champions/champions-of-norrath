#include "cd_file.cpp"

#include <cstdio>

int main() {
    if (sizeof gCdFile != 40) return 1;
    for (unsigned w : gCdFile)
        if (w) return 2;
    std::puts("cd file passed");
    return 0;
}
