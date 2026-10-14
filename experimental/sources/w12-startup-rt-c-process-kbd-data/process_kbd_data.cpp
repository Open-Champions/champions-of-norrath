/* Observed process_kbd_data at 0x002e8490, 584 bytes (a local symbol), pinned SLUS_205.65.
 * Called by KbmReaderThread for each 28-byte record of the data reply whose halfword at +4
 * is 1. It uses the keyboard slot table of GLibData (0x008ebdd8): count at +64, slot i at
 * 136 + 272 * i with its identifier halfword at +4 of the slot. It returns an int.
 *   1. If the count is not positive, or no slot's identifier equals the halfword at record
 *      +6 (signed compare, first match in order), or the count read again no longer
 *      exceeds the index, the result is 0x80000000.
 *   2. If the word at record +12 is nonzero it is stored in the slot's word at +16 and is
 *      also the result.
 *   3. If the byte at record +22 is 1, the record's word at +16 is cleared, the slot's word
 *      at +16 becomes 0x8000000c and so is the result.
 *   4. Otherwise it waits on the semaphore in the word at GLibData +44, calls
 *      CheckforModifierKeyEvents(slot, record), copies record bytes 10 and 11 to slot bytes
 *      32 and 33, and compacts the nonzero ones of the six bytes at record +22..+27 into
 *      the bytes from record +20 on, storing how many into the record's word at +16.
 *      generate_key_release_events(slot, record) then generate_key_press_events(slot,
 *      record) are called and each result is stored in the slot's word at +16. The
 *      compacted bytes are copied (length = the record's word at +16) to slot +260 and
 *      that length is stored at slot +256. After SignalSema on the same semaphore it walks
 *      the 32 bytes at slot +224 as a bit set: for every set bit number n (byte * 8 + bit)
 *      it formats "%x " into a local buffer and appends it to a text buffer that starts
 *      empty. The text is never used. The result is the slot's word at +16.
 * Inferred: slot +224 is a bitmap of held keys, +256/+260 the list of keys down; the text
 * walk is leftover debug output. The two buffers are 208 and 112 bytes in the original frame;
 * a full bitmap would overflow the first, which the original does not guard against.
 * Unknown: the layouts of the record and of the three callees' contracts beyond their
 * argument lists, and the meaning of the 0x8000000c / 0x80000000 results.
 */
extern "C" {
extern unsigned char GLibData[];
int WaitSema(int semaphore);
int SignalSema(int semaphore);
void* memcpy(void* destination, const void* source, unsigned long size);
int sprintf(char* buffer, const char* format, ...);
char* strcat(char* destination, const char* source);
void CheckforModifierKeyEvents(unsigned char* slot, unsigned char* record);
int generate_key_release_events(unsigned char* slot, unsigned char* record);
int generate_key_press_events(unsigned char* slot, unsigned char* record);

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

int process_kbd_data(void* data_record) {
    unsigned char* const record = static_cast<unsigned char*>(data_record);
    unsigned char* const g = GLibData;
    char text[208];
    char piece[112];
    const int count = *reinterpret_cast<AliasedInt*>(g + 64);
    if (count <= 0) return static_cast<int>(0x80000000u);
    const short identifier = *reinterpret_cast<AliasedShort*>(record + 6);
    int index = 0;
    while (*reinterpret_cast<AliasedShort*>(g + 140 + 272 * index) != identifier) {
        ++index;
        if (!(index < count)) return static_cast<int>(0x80000000u);
    }
    if (!(index < *reinterpret_cast<AliasedInt*>(g + 64))) return static_cast<int>(0x80000000u);
    unsigned char* const slot = g + 136 + 272 * index;
    const int skipped = *reinterpret_cast<AliasedInt*>(record + 12);
    if (skipped != 0) {
        *reinterpret_cast<AliasedInt*>(slot + 16) = skipped;
        return *reinterpret_cast<AliasedInt*>(record + 12);
    }
    if (record[22] == 1) {
        *reinterpret_cast<AliasedInt*>(record + 16) = 0;
        *reinterpret_cast<AliasedInt*>(slot + 16) = 0x8000000c;
        return 0x8000000c;
    }
    WaitSema(*reinterpret_cast<AliasedInt*>(g + 44));
    CheckforModifierKeyEvents(slot, record);
    slot[32] = record[10];
    slot[33] = record[11];
    {
        const unsigned char* source = record + 22;
        unsigned char* destination = record + 20;
        int kept = 0;
        for (int remaining = 5; remaining >= 0; --remaining) {
            const unsigned char key = *source++;
            if (key != 0) {
                *destination++ = key;
                ++kept;
            }
        }
        *reinterpret_cast<AliasedInt*>(record + 16) = kept;
    }
    *reinterpret_cast<AliasedInt*>(slot + 16) = generate_key_release_events(slot, record);
    *reinterpret_cast<AliasedInt*>(slot + 16) = generate_key_press_events(slot, record);
    memcpy(slot + 260, record + 20, *reinterpret_cast<AliasedInt*>(record + 16));
    *reinterpret_cast<AliasedInt*>(slot + 256) = *reinterpret_cast<AliasedInt*>(record + 16);
    SignalSema(*reinterpret_cast<AliasedInt*>(GLibData + 44));
    text[0] = 0;
    for (int byte = 0; byte < 32; ++byte) {
        for (int bit = 0; bit < 8; ++bit) {
            if ((slot[224 + byte] >> bit) & 1) {
                sprintf(piece, "%x ", byte * 8 + bit);
                strcat(text, piece);
            }
        }
    }
    return *reinterpret_cast<AliasedInt*>(slot + 16);
}
}
