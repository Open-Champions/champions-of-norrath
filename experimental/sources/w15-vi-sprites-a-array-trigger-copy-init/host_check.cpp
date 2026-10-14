// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "init.cpp"
#include <cstring>
static int dpRet, alRet, dpCalls, alCalls, delCalls; static void* delSeen;
extern "C" int DestroyPool__t7VIArray1Z16VIHSpriteTrigger(void*) { ++dpCalls; return dpRet; }
extern "C" int Allocate__t7VIArray1Z16VIHSpriteTrigger(void* a) { ++alCalls; static_cast<unsigned*>(a)[1] = arena_base + 0x1000; return alRet; }
extern "C" void __builtin_vec_delete(void* p) { ++delCalls; delSeen = p; }
int main() {
    unsigned array[2] = {7, 0}, source[2] = {3, arena_base + 0x100};
    for (int i = 0; i < 48; ++i) arena[0x100 + i] = static_cast<unsigned char>(i + 1);
    std::memset(arena + 0x1000, 0xee, 0x100);
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(array, source) != 0) return 1;
    if (array[0] != 3 || array[1] != arena_base + 0x1000 || dpCalls || delCalls || alCalls != 1) return 2;
    if (std::memcmp(arena + 0x1000, arena + 0x100, 48) != 0 || arena[0x1000 + 48] != 0xee) return 3;
    // old storage: destroyed, released, then replaced
    array[0] = 9; array[1] = arena_base + 0x200;
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(array, source) != 0 || dpCalls != 1 || delCalls != 1 || delSeen != arena + 0x200) return 4;
    dpRet = -1; array[0] = 9; array[1] = arena_base + 0x200; alCalls = 0;
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(array, source) != -1 || alCalls || array[0] != 9) return 5;
    dpRet = 0; alRet = -1; array[1] = 0;
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(array, source) != -1 || alCalls != 1) return 6;
    alRet = 0; source[0] = 0; std::memset(arena + 0x1000, 0xee, 0x100);
    if (Init__t7VIArray1Z16VIHSpriteTriggerRCt7VIArray1Z16VIHSpriteTrigger(array, source) != 0 || array[0] != 0 || arena[0x1000] != 0xee) return 7;
    return 0;
}
