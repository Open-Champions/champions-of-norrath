#include "init_pre_select.cpp"

#include <cstdio>
#include <cstring>

int curPlayer;
int selectMode = 7;
int lockAlpha = 7;
int flareAlpha = 7;
float delta = 1.0f;
signed char playerSelectInfo[41 * 4];
static int calls, ca, cb;
extern "C" void initPosition__Fii(int a, int b) { ++calls; ca = a; cb = b; }

int main() {
    playerSelectInfo[32 + 41 * 2] = -5;
    playerSelectInfo[32 + 41 * 1] = 9;
    curPlayer = 2;
    initPreSelect__Fv();
    if (calls != 1 || ca != 0 || cb != -5) return 1;
    unsigned w;
    std::memcpy(&w, &delta, 4);
    if (w != 0x3d5d2f1au) return 2;
    if (selectMode != 0 || lockAlpha != 0 || flareAlpha != 0) return 3;
    curPlayer = 1;
    initPreSelect__Fv();
    if (calls != 2 || cb != 9) return 4;
    std::puts("init pre select passed");
    return 0;
}
