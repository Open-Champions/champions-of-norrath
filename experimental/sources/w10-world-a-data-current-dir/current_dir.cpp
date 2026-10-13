// Observed currentDir at 0x008e7100 (.bss), 4096 bytes. The span to the next symbol (retval) is 4096 bytes. The authored memory-card chdir code hands its address to the SIF and writes back 1024 bytes of it.
// Inferred: A path text buffer for memory-card directory operations.
// Unknown: The real size (only 1024 bytes are observed in use; the span may include padding).
extern "C" {
unsigned char currentDir[4096];
}
