// blockGetNmFree__Fv at 0x00144778, 96 bytes in the pinned image. This is the
// original-name entry point for the already published
// norrath_block_get_number_free (block-allocator-lifecycle-build).
// Observed: the image body walks the free lists of the three 16-byte rows of
// `region` (list head at row offset 12, next link in the first word of each
// node) and returns the total node count. The published body does the same, and
// docs/agents/w8-render-world-a-compare.py runs this entry point and the image
// function on identical randomized lists and requires equal results and memory.
// Inferred: the image function is a plain function, so this forwarder only
// supplies the name that the callers import; it adds no behavior.
// Unknown: whether the original had a second user of the row layout.
extern "C" {
int norrath_block_get_number_free();

int blockGetNmFree__Fv() {
    return norrath_block_get_number_free();
}
}
