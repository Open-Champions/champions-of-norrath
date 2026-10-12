#include "mc_run_cmd_no.cpp"

#include <cstdio>

int main() {
    if (sizeof(mcRunCmdNo) != 4 || mcRunCmdNo != 0) return 1;
    std::puts("mc run cmd no passed");
    return 0;
}
