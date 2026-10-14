#include "flags.cpp"
int main() {
    if (sizeof(flags) != 4) return 1;
    if (flags != 0) return 2;
    flags = 1;
    if (flags != 1) return 3;
    return 0;
}
