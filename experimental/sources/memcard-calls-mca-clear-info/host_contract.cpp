#include "clear_info.cpp"

#include <cstdio>

extern "C" int MCA_Present[8] = {};
extern "C" int MCA_Formatted[8] = {};

int main() {
    for (int index = 0; index < 8; ++index) {
        MCA_Present[index] = 7;
        MCA_Formatted[index] = 9;
    }
    MCA_ClearInfo__Fv();
    for (int index = 0; index < 8; ++index) {
        if (MCA_Present[index] != 0 || MCA_Formatted[index] != -1) return index + 1;
    }
    MCA_Present[3] = 4;
    MCA_Formatted[3] = 0;
    MCA_ClearInfo__Fv();
    if (MCA_Present[3] != 0 || MCA_Formatted[3] != -1) return 20;
    std::puts("mca clear info passed");
    return 0;
}
