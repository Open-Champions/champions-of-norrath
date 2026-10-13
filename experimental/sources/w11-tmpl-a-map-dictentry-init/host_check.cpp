#include "init.cpp"
static int clearRet, allocRet, trace, cap_seen;
extern "C" int Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* m) { trace = trace * 10 + 1; return clearRet; }
extern "C" int Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry(void* m) { trace = trace * 10 + 2; cap_seen = reinterpret_cast<int*>(m)[1]; return allocRet; }
int main() {
    int m[4] = {0x11, 0x22, 0x33, 0x44};
    clearRet = -3; trace = 0;
    if (Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(m, 9) != -1 || trace != 1 || m[1] != 0x22) return 1;
    int rets[4][2] = {{0, 0}, {5, 0}, {-1, -1}, {-100, -1}};
    for (int i = 0; i < 4; ++i) {
        clearRet = 0; allocRet = rets[i][0]; trace = 0; cap_seen = 0;
        if (Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(m, 17 + i) != rets[i][1] || trace != 12 || cap_seen != 18 + i || m[1] != 18 + i) return 2 + i;
    }
    return 0;
}
