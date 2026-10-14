// Observed _GLOBAL_$D$lastkey at 0x2cb480, extent 32: the matching destruction
// entry. It calls the unit's local __static_initialization_and_destruction_0
// with (0, 0xffff), which runs the VIKeyboard destructor with flag 2 on vik
// (0x8d86a0). The same local function, with the same body, also backs
// _GLOBAL_$I$lastkey (see global_init_lastkey.cpp). Returns nothing.
struct VIKeyboard;
extern "C" VIKeyboard vik;
extern "C" void __10VIKeyboard(VIKeyboard* self);
extern "C" void _$_10VIKeyboard(VIKeyboard* self, int flags);

namespace {
__attribute__((noipa)) void static_initialization_and_destruction_0(int initialize, int priority) {
    if (priority == 0xffff) {
        if (initialize == 1) __10VIKeyboard(&vik);
        if (initialize == 0) _$_10VIKeyboard(&vik, 2);
    }
}
}  // namespace

extern "C" void _GLOBAL_$D$lastkey() {
    static_initialization_and_destruction_0(0, 0xffff);
}
