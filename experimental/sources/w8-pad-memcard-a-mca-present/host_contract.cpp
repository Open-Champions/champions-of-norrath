#include "mca_present.cpp"

#include <cstdio>

int main() {
    if (sizeof MCA_Present != 32) return 1;
    for (int i = 0; i < 8; ++i) if (MCA_Present[i] != 0) return 2;
    if (sizeof(MCA_Present[0]) != 4) return 3;
    std::puts("mca present passed");
    return 0;
}
