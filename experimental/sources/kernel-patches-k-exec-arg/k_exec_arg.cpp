// Observed: _kExecArg at 0x00445d48, a 4-byte word. The image bytes are zero.
// InitTLBFunctions stores the result of its final entry query in it.
// Inferred: none.
// Unknown: its use after that store.
extern "C" {
unsigned _kExecArg = 0;
}
