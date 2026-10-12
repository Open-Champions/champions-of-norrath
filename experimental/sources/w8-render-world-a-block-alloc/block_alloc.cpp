// blockAlloc__Fi at 0x001441b8, 188 bytes in the pinned image. This is the
// original-name entry point for the already published norrath_block_alloc
// (block-alloc-source-v2).
// Observed: the image body scans the 16-byte rows of `region` for the first row
// whose size word is at least the argument and whose free-list head is not zero,
// returns null for the fourth row, pops the head, clears the row's size in bytes
// with memset and calls listSetCopyObject__FPv with the block. The published
// body does these steps; docs/agents/w8-render-world-a-compare.py runs this
// entry point with the published body against the image on randomized rows and
// requires equal results, memory and callee argument logs.
// Inferred: only the name differs; the forwarder adds no behavior.
// Unknown: what the fourth row represents beyond being the stop row.
extern "C" {
unsigned char* norrath_block_alloc(int requested_size);

unsigned char* blockAlloc__Fi(int requested_size) {
    return norrath_block_alloc(requested_size);
}
}
