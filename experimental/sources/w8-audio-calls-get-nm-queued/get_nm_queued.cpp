/* soundGetNmQueued__Fi at 0x0024f388 is 40 bytes. It returns the word at
 * offset 448 of stream record `index` (record size 452, base stream at
 * 0x007dd290). No bounds check: the index is multiplied and added as given.
 * What the word counts is read from the name only; the writer of offset 448
 * is outside this file.
 */
extern "C" {
extern unsigned char stream[];

int soundGetNmQueued__Fi(int index) {
    int count;
    __builtin_memcpy(&count, stream + index * 452 + 448, sizeof count);
    return count;
}
}
