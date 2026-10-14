#include "mc_front_end_import.cpp"

#include <cstdio>

int main() {
    if (sizeof(MC_FrontEndImport) != 1 || MC_FrontEndImport != 0) return 1;
    std::puts("mc front end import passed");
    return 0;
}
