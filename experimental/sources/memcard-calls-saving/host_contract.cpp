#include "saving.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_SavingOrLoading != 1 || MC_SavingOrLoading != 0) return 1;
    MC_SavingOrLoading = 1;
    if (MC_SavingOrLoading != 1) return 2;
    std::puts("saving or loading passed");
    return 0;
}
