// Observed scriptInPostAction at 0x00485097, one byte in .sdata with the
// initial value zero. The preceding bytes are the task text "Script" and its
// terminator (see script-amx-script-init). Under the reviewed authored_cpp
// profile a zero-initialised byte is placed in BSS, so the section differs from
// the reference; the value is the same. Its meaning is not established.
extern "C" {
unsigned char scriptInPostAction;
}
