#include <cstdio>
#include "getpid.cpp"
int main() {
    if (getpid() != 1 || getpid() != 1) { std::puts("getpid wrong"); return 1; }
    std::puts("getpid contract passed");
    return 0;
}
