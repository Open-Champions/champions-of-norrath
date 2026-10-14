// Observed _GLOBAL_$I$addpmlist__FPc at 0x26b3b8, extent 32: the compiler-made
// static-initialization entry of the translation unit that holds addpmlist.
// It calls the unit's own __static_initialization_and_destruction_0 with
// (1, 0xffff). That local function (0x26b2c0, 8 bytes) does nothing: the unit
// has no global that needs construction. There is no matching _GLOBAL_$D$ entry
// in the image. Returns nothing.
namespace {
__attribute__((noipa)) void static_initialization_and_destruction_0(int initialize, int priority) {
    (void)initialize;
    (void)priority;
}
}  // namespace

extern "C" void _GLOBAL_$I$addpmlist__FPc() {
    static_initialization_and_destruction_0(1, 0xffff);
}
