// Observed __default_terminate at 0x002f9ff8, 20 bytes. Calls abort() (0x002ff8c0) with no
// argument; the original reaches it as a tail call after a frame it does not use. It is the
// routine the terminate-handler word __terminate_func holds at start-up.
// Nothing was executed on the EE.
extern "C" {
void abort();

void __default_terminate() {
    abort();
}
}
