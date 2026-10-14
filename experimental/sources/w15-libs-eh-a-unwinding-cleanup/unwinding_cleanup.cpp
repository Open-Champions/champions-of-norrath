// Observed __unwinding_cleanup at 0x002facd8, 8 bytes: a bare return with no
// argument use, no memory access and no call. The only evidence for its role is
// its name and its place among the exception-runtime routines at
// 0x002fa920..0x002fcd00 (inferred: a hook point the unwinder can call and a
// debugger can break on; not confirmed).
extern "C" void __unwinding_cleanup() {}
