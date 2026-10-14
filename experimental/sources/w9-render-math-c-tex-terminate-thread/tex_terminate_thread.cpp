// Observed texTerminateThread__Fv at 0x00172978, 32 bytes. Loads the 32-bit value
// decompressThreadid (a small-data global, offset -4232 from GP 0x004879f0) and passes it
// to TerminateThread. The result is ignored; nothing is returned.
// Inferred: decompressThreadid holds the id of the texture decompression thread (the
// texDecompressThread unit reads it). Unknown: whether the id is cleared afterwards
// (it is not, here).
extern int decompressThreadid;
extern "C" int TerminateThread(int thread);

extern "C" void texTerminateThread__Fv() {
    TerminateThread(decompressThreadid);
}
