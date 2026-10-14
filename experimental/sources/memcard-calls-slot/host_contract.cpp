#include "slot.cpp"

#include <cstdio>

int main() {
    if (sizeof MCA_Slot != 4 || MCA_Slot != 0) return 1;
    MCA_Slot = -1;
    if (MCA_Slot != -1) return 2;
    std::puts("mca slot passed");
    return 0;
}
