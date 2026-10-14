// Host check only; not a build input.
#include "init.cpp"
#include <cstdio>
static int clearRet, allocRet, clearCalls, allocCalls, wordAtAlloc, order;
extern "C" int Clear__t7VIArray1Z14VIResourceElem(void* c) { ++clearCalls; order = order * 10 + 1; return clearRet; }
extern "C" int Allocate__t7VIArray1Z14VIResourceElem(void* c) { ++allocCalls; order = order * 10 + 2; wordAtAlloc = static_cast<int*>(c)[0]; return allocRet; }
int main() {
    bool ok = true;
    int rets[3] = {-5, 0, 7};
    for (int cr : rets) for (int ar : rets) {
        int words[4] = {11, 22, 33, 44};
        clearRet = cr; allocRet = ar; clearCalls = allocCalls = order = 0; wordAtAlloc = 0;
        int r = Init__t7VIArray1Z14VIResourceElemi(words, 99);
        int want = cr < 0 ? -1 : (ar < 0 ? -1 : 0);
        if (r != want) ok = false;
        if (clearCalls != 1 || allocCalls != (cr < 0 ? 0 : 1)) ok = false;
        if (cr < 0) { if (order != 1 || words[0] != (0 == 0 ? 11 : 22)) ok = false; }
        else if (order != 12 || wordAtAlloc != 99 || words[0] != 99) ok = false;
        if (words[1] != (1 == 0 ? 11 : 22) || words[2] != 33 || words[3] != 44) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
