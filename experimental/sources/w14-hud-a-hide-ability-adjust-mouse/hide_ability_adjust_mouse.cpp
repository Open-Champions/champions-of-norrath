// Observed: hideAbilityAdjustMouse is the 8-byte bss object at 0x00486a90 (the
// next symbol starts eight bytes later). The only authored reader indexes it by
// a shop side, so it is two ints. The image bytes are zero at load.
// Unknown: what writes it; no code in the image stores to this address.
extern "C" int hideAbilityAdjustMouse[2] = {};
static_assert(sizeof(hideAbilityAdjustMouse) == 8, "hideAbilityAdjustMouse");
