/* soundClearStreamQue__Fi at 0x0024f0a8 is 140 bytes. `stream` (0x007dd290)
 * holds 452-byte records. Within record `index`, the word at 448 counts
 * queue entries; the entries are 48-byte blocks starting at offset 64.
 * While the loop index is below the count (re-read from the record after
 * every clear), the next 48-byte block is cleared with memset. Then the
 * word at offset 36 and the count at 448 are set to zero. A count of zero
 * or less clears no block. The record's other bytes are untouched.
 */
extern "C" {
extern unsigned char stream[];
void* memset(void* destination, int value, unsigned long count);

void soundClearStreamQue__Fi(int index) {
    unsigned char* record = stream + index * 452;
    volatile int* count = reinterpret_cast<volatile int*>(record + 448);
    if (*count > 0) {
        unsigned char* entry = record + 64;
        int cleared = 0;
        do {
            memset(entry, 0, 48);
            entry += 48;
            ++cleared;
        } while (cleared < *count);
    }
    *reinterpret_cast<int*>(record + 36) = 0;
    *count = 0;
}
}
