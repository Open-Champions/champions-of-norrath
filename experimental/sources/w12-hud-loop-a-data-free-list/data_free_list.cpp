// Observed freeList at 0x00486a68, 4 bytes, section .sbss/.bss (local b).
// A local uninitialised word of 4 bytes. The next symbol, freeNodes, starts 4 bytes later. floorClear__Fv writes it and floorAdd__FG6Point3sfii reads and writes it, as whole words.
// Inferred: A 32-bit address word (callers declare it as a pointer), the head of a free list; zero-initialised here.
// Unknown: Contents at run time; this unit does not set them.
extern "C" {
unsigned freeList;
}
