// Observed __terminate_func at 0x004480d8, 4 bytes of initialized data. The image holds
// 0x002f9ff8 there, the address of __default_terminate (a 20-byte routine that calls abort).
// __terminate loads this word and calls it. Inferred only: that this is the installed terminate
// handler, from the symbol name and that use. Unknown: who else writes it.
extern "C" {
void __default_terminate();
void (*__terminate_func)() = __default_terminate;
}
