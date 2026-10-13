// Host check for init__4itemv. Not an EE plan input. Pointers in the tables are
// small ids here (the source reads them as 32-bit words), mapped to names below.
#include "item_init.cpp"
#include <cstring>
#include <string>
alignas(8) unsigned char _4item$globalItem[16 * 3];
alignas(8) unsigned char _4item$itemTable[64 * 4];
static std::string ev;
static unsigned resource = 100;
static const char* const names[] = {"", "a", "b", "c", "i0", "i1", "equipmnt.lmp", "compbow_shot.anm"};
static std::string N(const char* p) {
    __UINTPTR_TYPE__ v = reinterpret_cast<__UINTPTR_TYPE__>(p);
    return v < 8 ? names[v] : std::string(p);
}
extern "C" {
void* lumpFindResource__FPcT0(char* l, char* n) { ev += N(l) + "/" + N(n) + ";"; return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(++resource)); }
void* create__4itemPcPQ24item11descriptionii(char* name, unsigned char* d, int a, int b) {
    ev += "C:" + N(name) + "," + std::to_string(a) + "," + std::to_string(b) + ",";
    for (int i : {32, 33, 36, 37, 44, 52, 60, 16, 22, 24, 30}) ev += std::to_string(d[i]) + " ";
    ev += "d4=" + std::to_string(d[4]) + ";";
    memset(d, 0x5a, 72);
    return nullptr;
}
}
static void put(unsigned char* base, int off, unsigned v) { memcpy(base + off, &v, 4); }
static unsigned get(unsigned char* base, int off) { unsigned v; memcpy(&v, base + off, 4); return v; }
int main() {
    put(_4item$globalItem, 0, 1); put(_4item$globalItem, 4, 2);
    put(_4item$globalItem, 16, 3); put(_4item$globalItem, 20, 1);
    put(_4item$itemTable, 0, 4); _4item$itemTable[30] = 4;
    put(_4item$itemTable, 64, 5); _4item$itemTable[64 + 30] = 3;
    init__4itemv();
    if (get(_4item$globalItem, 8) != 101 || get(_4item$globalItem, 12) != 102 || get(_4item$globalItem, 24) != 103 || get(_4item$globalItem, 28) != 104) return 1;
    const std::string first = "equipmnt.lmp/a;equipmnt.lmp/b;equipmnt.lmp/c;equipmnt.lmp/a;";
    if (ev.compare(0, first.size(), first) != 0) return 2;
    std::string rest = ev.substr(first.size());
    // the first item's d[4] is uninitialized stack; the second sees the 1 stored after the first create
    if (rest.find("C:i0,-1,0,255 255 255 255 255 255 255 0 0 0 0 d4=") != 0) return 3;
    if (rest.find("equipmnt.lmp/compbow_shot.anm;C:i1,-1,0,255 255 255 255 255 255 255 0 0 0 0 d4=1;") == std::string::npos) return 4;
    if (get(_4item$itemTable, 56) != 105 || get(_4item$itemTable, 64 + 56) != 0) return 5;
    return 0;
}
