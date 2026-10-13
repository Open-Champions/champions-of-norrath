// Observed blockFree__FPv at 0x001447d8, 88 bytes in SLUS_205.65. It works on the 4-row,
// 16-bytes-per-row table `region` (0x00333d58, a local data symbol of 0x40 bytes). Each row holds
// four words. Observed from the image: row 0 starts as {0x180, 0x200, 0, 0}, row 1 as
// {0x500, 0x200, 0, 0}, row 2 as {0xbb8, 0x20, 0, 0} and row 3 as {0x7fffffff, 0, 0, 0}.
// The fourth word of a row is a free-list head (blockAlloc pops from it and the routine below
// pushes onto it). The third word is compared with the freed pointer here.
//
// Behavior: row index k is the first k, counting from 0, for which pointer < word2 of row k+1
// (the comparison is unsigned). The pointer is then stored in the head word of row k and the
// previous head is written into the first word of the freed block. No range or null check
// exists, so a pointer at or above word2 of row 3 keeps reading beyond the table.
// Inferred: rows are size classes, word2 of rows 1..3 being the start address of that class's
// pool (the all-zero image values are filled in at run time) and word1 a block count. Unknown:
// the exact meaning of word0 and word1 beyond their use by blockAlloc, which is not part of
// this file, and which source file originally owned the table.
struct BlockRegionRow {
    int word0;
    int word1;
    __UINTPTR_TYPE__ word2;
    void* head;
};
extern "C" BlockRegionRow norrath_block_region_view[];

extern "C" void blockFree__FPv(void* object) {
    __UINTPTR_TYPE__ pointer = reinterpret_cast<__UINTPTR_TYPE__>(object);
    int row = 0;
    while (!(pointer < norrath_block_region_view[row + 1].word2)) row++;
    void* previous = norrath_block_region_view[row].head;
    norrath_block_region_view[row].head = object;
    *static_cast<void**>(object) = previous;
}
