/* Observed _init_signal_r at 0x00303998, 112 bytes. Argument: a reentrancy record whose
 * word at offset 468 points at a table of 32 handler words.
 *   - A nonzero table pointer returns 0 and changes nothing.
 *   - Otherwise _malloc_r(record, 128) is called and its result is stored in the table
 *     pointer word, even when it is null. A null result returns -1.
 *   - Otherwise the 32 words, from the last to the first, are set to zero and 0 is
 *     returned.
 * _raise_r calls it when it finds no table.
 * Unknown: the rest of the record's layout. Compiled output is not claimed to match the
 * original.
 */
extern "C" {
struct Reent {
    int error_number;
    unsigned char unused_4[464];
    unsigned* signal_handlers;
};
void* _malloc_r(Reent* reent, unsigned size);

int _init_signal_r(Reent* reent) {
    if (reent->signal_handlers != 0) return 0;
    unsigned* const table = static_cast<unsigned*>(_malloc_r(reent, 128));
    reent->signal_handlers = table;
    if (table == 0) return -1;
    for (int index = 31; index >= 0; --index) table[index] = 0;
    return 0;
}
}
