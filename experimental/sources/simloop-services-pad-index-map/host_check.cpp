#include <cstdio>
extern "C" unsigned char padIndexMap[4] = {9, 9, 9, 9};
extern "C" void padInitPadIndexMap__Fv();
int main() {
    padInitPadIndexMap__Fv();
    if (padIndexMap[0] != 0 || padIndexMap[1] != 1 || padIndexMap[2] != 2 || padIndexMap[3] != 3)
        return 1;
    std::puts("pad index map contract passed");
    return 0;
}
