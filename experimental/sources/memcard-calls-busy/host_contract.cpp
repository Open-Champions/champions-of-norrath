#include "busy.cpp"

#include <cstdio>

extern "C" int disableMC = 0;
extern "C" int asyncMode = 0;
extern "C" int errorCondition = 0;

int main() {
    disableMC = 4;
    asyncMode = -1;
    errorCondition = 1;
    if (MC_Busy__Fv() != 0) return 1;
    disableMC = 0;
    asyncMode = 2;
    errorCondition = 0;
    if (MC_Busy__Fv() != 1) return 2;
    asyncMode = -1;
    errorCondition = 0;
    if (MC_Busy__Fv() != 0) return 3;
    errorCondition = -5;
    if (MC_Busy__Fv() != 1) return 4;
    std::puts("mc busy passed");
    return 0;
}
