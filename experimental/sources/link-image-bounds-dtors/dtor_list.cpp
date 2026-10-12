// Observed __DTOR_LIST__ at 0x0031f3ec. The .dtors section is 52 bytes:
// an all-ones count word, 11 function pointers, and a trailing zero.
// The pointers are the named functions. This unit does not define them.
extern "C" {
void _GLOBAL_$D$objectIgnoreFlags();
void _GLOBAL_$D$worldTileEntityLightScale();
void _GLOBAL_$D$inventoryLoad__Fv();
void _GLOBAL_$D$shopActiveFrame();
void _GLOBAL_$D$displayQuestInfo__FP11questStructi();
void _GLOBAL_$D$optionMenu();
void _GLOBAL_$D$wraithResourceMap();
void _GLOBAL_$D$orc1ResourceMap();
void _GLOBAL_$D$wesResourceMap();
void _GLOBAL_$D$soundSfxVolume();
void _GLOBAL_$D$lastkey();
void (*__DTOR_LIST__[])() = {
    reinterpret_cast<void (*)()>(static_cast<unsigned long>(-1)),
    _GLOBAL_$D$objectIgnoreFlags,
    _GLOBAL_$D$worldTileEntityLightScale,
    _GLOBAL_$D$inventoryLoad__Fv,
    _GLOBAL_$D$shopActiveFrame,
    _GLOBAL_$D$displayQuestInfo__FP11questStructi,
    _GLOBAL_$D$optionMenu,
    _GLOBAL_$D$wraithResourceMap,
    _GLOBAL_$D$orc1ResourceMap,
    _GLOBAL_$D$wesResourceMap,
    _GLOBAL_$D$soundSfxVolume,
    _GLOBAL_$D$lastkey,
    nullptr,
};
}
