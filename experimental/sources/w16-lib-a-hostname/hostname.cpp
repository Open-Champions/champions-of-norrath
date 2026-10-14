// Observed data object, pinned SLUS_205.65: hostname at 0x0118e6c0, 128 bytes (symbol size), global, all bytes zero in the
// image. The next object, cd, starts at 0x0118e740, exactly 128 bytes later. The network start-up thread passes it as the
// 128-byte name output of sceInetAddress2Name and, when no name is returned, copies its address text into it before printing.
// Inferred: a C string buffer for this machine's network name; the unit only reserves the storage.
// Unknown: other readers of the buffer and whether anything writes it before the thread runs.
extern "C" {
alignas(16) char hostname[128];
}
