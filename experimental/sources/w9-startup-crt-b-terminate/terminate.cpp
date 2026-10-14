// Observed __terminate at 0x002fa010, 24 bytes. It loads the code pointer held
// in __terminate_func (0x004480d8, a global data word) and calls it. Nothing is
// restored after the call: the frame only saves ra and the routine has no
// return path, so the callee is treated as not returning. __pure_virtual tail
// calls this routine. Inferred: the word is the installed terminate handler,
// following the symbol name only; the neighbouring __default_terminate
// (0x002f9ff8) is not authored here. Unknown: what the word holds at start-up.
extern "C" {
extern void (*__terminate_func)() __attribute__((noreturn));

void __terminate() __attribute__((noreturn));
void __terminate() {
    __terminate_func();
}
}
