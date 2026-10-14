#include "mca_reset.cpp"

#include <cstdio>

int queueStart = 3, queueEnd = 4, checkResults = 5, clearBytes = 6;

int main() {
    MCA_Reset__Fv();
    if (queueStart != 0 || queueEnd != 0 || checkResults != 0 || clearBytes != -1) return 1;
    std::puts("mca reset passed");
    return 0;
}
