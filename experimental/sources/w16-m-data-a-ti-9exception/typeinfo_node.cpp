// Observed __ti9exception at 0x00486de8, 8 bytes of zero-initialized (bss) type-info node storage. It is the root node
// for the exception hierarchy: __tf9bad_alloc and __tf10bad_typeid register against it as their base.
// Inferred from those registration calls. Unknown: the node fields after the first word, which __rtti_user writes.
extern "C" {
unsigned char __ti9exception[8] __attribute__((aligned(4))) = {};
}
