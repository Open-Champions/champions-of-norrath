// blockAllocInit__Fv at 0x001440d8, 220 bytes in the pinned image. This is the
// original-name entry point for the already published norrath_block_alloc_init
// (block-alloc-source-v2).
// Observed: the image body clears 0xe7700 bytes at dataBlock with memset, then
// for each of the three rows of `region` threads a free list of `count` nodes
// of `size` bytes through the pool, stores the row base and head, and writes the
// final cursor and pool base into the fourth row. The published body does these
// steps; docs/agents/w8-render-world-a-compare.py runs this entry point with the
// published body against the image on randomized row tables.
// Inferred: only the name differs; the forwarder adds no behavior.
// Unknown: why the fourth row holds the pool base.
extern "C" {
void norrath_block_alloc_init();

void blockAllocInit__Fv() {
    norrath_block_alloc_init();
}
}
