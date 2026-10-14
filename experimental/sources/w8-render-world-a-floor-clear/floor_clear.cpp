// floorClear__Fv at 0x00191de0, 276 bytes in the pinned image. This is the
// original-name entry point for the already published norrath_floor_clear
// (game-floor-clear-source).
// Observed: the image body zeroes the sprite count, clears the 128-record sprite
// packet pool (768-byte records) with memset, links the records through the word
// at offset 752 into the free list and stores its head, clears the 3600-byte
// sprite grid with memset, writes the transfer command word (derived from the
// length of the VU program between vu1floorstart and vu1floorend) into the word
// at offset 744 of each record, and zeroes the fade packet word. The published
// body does the same; docs/agents/w8-render-world-a-compare.py runs this entry
// point with the published body against the image on randomized memory and
// requires equal stores and equal memset arguments.
// Inferred: only the name differs; the forwarder adds no behavior.
// Unknown: the meaning of the command word beyond its derivation from the
// program length.
extern "C" {
void norrath_floor_clear();

void floorClear__Fv() {
    norrath_floor_clear();
}
}
