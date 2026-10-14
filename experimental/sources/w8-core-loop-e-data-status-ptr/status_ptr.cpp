// Observed statusPtr at 0x00486af4. Four bytes in .sbss, so zero at load.
// The text accesses it only with word loads and word stores (two loads, five
// stores). The name suggests a pointer, but its type is not established, so
// it is declared as a plain word.
extern "C" {
unsigned statusPtr;
}
