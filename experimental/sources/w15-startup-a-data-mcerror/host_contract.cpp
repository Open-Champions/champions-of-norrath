#include "mcerror.cpp"

#include <cstdio>

int main() {
    if (sizeof mcerror != 4 || mcerror != 0) return 1;
    mcerror = 1;
    if (mcerror != 1) return 2;
    std::puts("mcerror contract passed");
    return 0;
}
