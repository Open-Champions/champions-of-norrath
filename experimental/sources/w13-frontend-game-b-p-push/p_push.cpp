// Observed P_Push__Fv at 0x00180578, extent 8: returns at once. It reads and
// writes nothing, and takes no arguments in registers that it uses.
// Unknown: why callers invoke it (the name suggests a stack push that the
// shipped build compiled away; this is not established).
extern "C" void P_Push__Fv() {}
