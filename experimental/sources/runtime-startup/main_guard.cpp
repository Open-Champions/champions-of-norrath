// Pinned __main at 0x002f9fa8: set the BSS guard before dispatch, including
// reentrant calls from a constructor. This is not the game's main function.
extern "C" void __do_global_ctors();
extern "C" void __main() {
    static unsigned initialized;
    if (initialized == 0) {
        initialized = 1;
        __do_global_ctors();
    }
}
