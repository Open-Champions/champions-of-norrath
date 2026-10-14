// Observed: VIEEUnloadIrx__Fi at 0x010f9cd8 is 56 bytes. Argument: a module id. It calls
// sceSifStopModule(id, 0, 0, &local_word) and then sceSifUnloadModule(id); the result of the
// second call is left in the return register by the original, and nothing else is done
// with the stop result or the local word.
// Inferred: the stop call gives the module a chance to shut down before it is freed on the
// IOP; the id is the value sceSifLoadModule returned.
// Unknown: whether the original declares an int return (the register is only incidentally
// live); this unit returns nothing, like its caller's declaration.
extern "C" {
int sceSifStopModule(int id, int length, const char* arguments, int* result);
int sceSifUnloadModule(int id);

void VIEEUnloadIrx__Fi(int id) {
    int result;
    sceSifStopModule(id, 0, nullptr, &result);
    sceSifUnloadModule(id);
}
}
