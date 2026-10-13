#include "init.cpp"
static int clearRet, allocRet, trace, cap_seen;
extern "C" int Clear__t6VIPool1ZP16VIStaticLighting(void* pool) { trace = trace * 10 + 1; return clearRet; }
extern "C" int Allocate__t6VIPool1ZP16VIStaticLighting(void* pool) { trace = trace * 10 + 2; cap_seen = reinterpret_cast<int*>(pool)[1]; return allocRet; }
int main() {
    int pool[4] = {0x11, 0x22, 0x33, 0x44};
    clearRet = -3; trace = 0;
    if (Init__t6VIPool1ZP16VIStaticLightingi(pool, 9) != -1 || trace != 1 || pool[1] != 0x22) return 1;
    int rets[4][3] = {{0, 0, 0}, {5, 0, 0}, {-1, -1, 0}, {-100, -1, 0}};
    for (int i = 0; i < 4; ++i) {
        clearRet = 0; allocRet = rets[i][0]; trace = 0; cap_seen = 0;
        int r = Init__t6VIPool1ZP16VIStaticLightingi(pool, 17 + i);
        if (r != rets[i][1] || trace != 12 || cap_seen != 17 + i || pool[1] != 17 + i) return 2 + i;
    }
    clearRet = 4;
    allocRet = 0; trace = 0;
    if (Init__t6VIPool1ZP16VIStaticLightingi(pool, 3) != 0 || trace != 12) return 9;
    return 0;
}
