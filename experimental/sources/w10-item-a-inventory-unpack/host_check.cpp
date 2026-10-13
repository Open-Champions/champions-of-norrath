// Host check for inventoryUnpack__Fiib with recording callees. Not an EE plan input.
#include "inventory_unpack.cpp"
#include <cstring>
#include <string>
unsigned char inventory[464] __attribute__((aligned(4)));
unsigned char statsScreen[232];
unsigned char animState[3 * 264];
static std::string ev;
extern "C" {
void animInitState__FP14AnimationState(void* s) { ev += "I" + std::to_string(static_cast<unsigned char*>(s) - animState); }
void animGlobalSync__FP14AnimationState(void* s) { ev += "Y" + std::to_string(static_cast<unsigned char*>(s) - animState); }
void inventorySetAnim__FiP14AnimationStatei(int p, void* s, int z) { ev += "A" + std::to_string(p) + "," + std::to_string(static_cast<unsigned char*>(s) - animState) + "," + std::to_string(z); }
void shopUnpackToGrid__Fiii(int a, int b, int c) { ev += "P" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c); }
}
static int w(const unsigned char* p) { int v; std::memcpy(&v, p, 4); return v; }
static float f(const unsigned char* p) { float v; std::memcpy(&v, p, 4); return v; }
int main() {
    std::memset(inventory, 0xee, sizeof inventory);
    inventoryUnpack__Fiib(2, 1, false);
    if (ev != "I528A2,528,1Y528P2,1,0") return 1;
    const unsigned char* s = inventory + 116;
    if (w(s) != 320 || w(s + 52) != 2 || w(s + 56) != 2 || w(s + 76) != 0) return 2;
    if (f(s + 68) != 420.0f || f(s + 72) != 200.0f) return 3;
    if (w(statsScreen + 116 + 52) != 2 || w(statsScreen + 52) != 0) return 4;
    if (w(inventory + 52) != (int)0xeeeeeeee) return 5;       // only the selected record changes
    ev.clear();
    inventoryUnpack__Fiib(0, 0, true);
    if (ev != "I0A0,0,1Y0") return 6;                         // no shop call
    if (w(inventory) != 0 || f(inventory + 68) != 100.0f || w(statsScreen + 52) != 0) return 7;
    return 0;
}
