// Observed _GLOBAL_$I$lastkey at 0x2cb460, extent 32: static-initialization entry
// of the keyboard translation unit. It calls the unit's local
// __static_initialization_and_destruction_0 (0x2cb1d0) with (1, 0xffff). That
// function, when the priority is 0xffff and the first argument is 1, runs the
// VIKeyboard constructor on the global keyboard object vik (0x8d86a0); with
// first argument 0 it runs the destructor with flag 2. Any other priority does
// nothing. The constructor and destructor are separate functions
// (0x2cbef0, 0x2cbf00) that this source imports. Returns nothing.
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

extern "C" void _GLOBAL_$I$lastkey() {
    static_initialization_and_destruction_0(1, 0xffff);
}
