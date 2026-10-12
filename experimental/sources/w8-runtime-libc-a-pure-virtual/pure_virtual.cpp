// Observed __pure_virtual at 0x002f9fe0, 24 bytes. The body is a tail call to
// __terminate at 0x002fa010 with no argument change, entered through a
// 16-byte frame that only saves and restores ra. The vtable slot a pure
// virtual call lands on points here; nothing else is done.
// Unknown: __terminate's behavior is outside this unit.
extern "C" {
void __terminate();

void __pure_virtual() {
    __terminate();
}
}
