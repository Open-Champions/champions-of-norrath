// Host check only; not a build input.
#include "init.cpp"
static int clearRet, allocRet, clearCalls, allocCalls, countAtAlloc; static void *clearSeen, *allocSeen;
extern "C" int Clear__t7VIArray1Z20VICSpritePartEmitter(void* a) { ++clearCalls; clearSeen = a; return clearRet; }
extern "C" int Allocate__t7VIArray1Z20VICSpritePartEmitter(void* a) { ++allocCalls; allocSeen = a; countAtAlloc = *static_cast<int*>(a); return allocRet; }
int main() {
    int array[4] = {99, 98, 97, 96};
    if (Init__t7VIArray1Z20VICSpritePartEmitteri(array, 5) != 0 || array[0] != 5 || countAtAlloc != 5 || clearSeen != array || allocSeen != array) return 1;
    allocRet = -3; if (Init__t7VIArray1Z20VICSpritePartEmitteri(array, 6) != -1 || array[0] != 6) return 2;
    clearRet = -1; allocCalls = 0; array[0] = 1;
    if (Init__t7VIArray1Z20VICSpritePartEmitteri(array, 7) != -1 || array[0] != 1 || allocCalls) return 3;
    clearRet = 4; allocRet = 2;
    if (Init__t7VIArray1Z20VICSpritePartEmitteri(array, 8) != 0 || array[0] != 8) return 4;
    return 0;
}
