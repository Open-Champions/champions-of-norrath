#include "dtor_list.cpp"

extern "C" void _GLOBAL_$D$objectIgnoreFlags() {}
extern "C" void _GLOBAL_$D$worldTileEntityLightScale() {}
extern "C" void _GLOBAL_$D$inventoryLoad__Fv() {}
extern "C" void _GLOBAL_$D$shopActiveFrame() {}
extern "C" void _GLOBAL_$D$displayQuestInfo__FP11questStructi() {}
extern "C" void _GLOBAL_$D$optionMenu() {}
extern "C" void _GLOBAL_$D$wraithResourceMap() {}
extern "C" void _GLOBAL_$D$orc1ResourceMap() {}
extern "C" void _GLOBAL_$D$wesResourceMap() {}
extern "C" void _GLOBAL_$D$soundSfxVolume() {}
extern "C" void _GLOBAL_$D$lastkey() {}

int main() {
    unsigned long first = reinterpret_cast<unsigned long>(__DTOR_LIST__[0]);
    if (first != static_cast<unsigned long>(-1)) return 1;
    int count = 0;
    for (int i = 1; __DTOR_LIST__[i] != nullptr; ++i) {
        count += 1;
        if (count > 200) return 2;
    }
    if (count != 11) return 3;
    if (__DTOR_LIST__[1] != _GLOBAL_$D$objectIgnoreFlags) return 100 + 1;
    if (__DTOR_LIST__[2] != _GLOBAL_$D$worldTileEntityLightScale) return 100 + 2;
    if (__DTOR_LIST__[3] != _GLOBAL_$D$inventoryLoad__Fv) return 100 + 3;
    if (__DTOR_LIST__[4] != _GLOBAL_$D$shopActiveFrame) return 100 + 4;
    if (__DTOR_LIST__[5] != _GLOBAL_$D$displayQuestInfo__FP11questStructi) return 100 + 5;
    if (__DTOR_LIST__[6] != _GLOBAL_$D$optionMenu) return 100 + 6;
    if (__DTOR_LIST__[7] != _GLOBAL_$D$wraithResourceMap) return 100 + 7;
    if (__DTOR_LIST__[8] != _GLOBAL_$D$orc1ResourceMap) return 100 + 8;
    if (__DTOR_LIST__[9] != _GLOBAL_$D$wesResourceMap) return 100 + 9;
    if (__DTOR_LIST__[10] != _GLOBAL_$D$soundSfxVolume) return 100 + 10;
    if (__DTOR_LIST__[11] != _GLOBAL_$D$lastkey) return 100 + 11;
    return 0;
}
