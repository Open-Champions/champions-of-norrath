/* Observed process_mouse_data at 0x002e86d8, 448 bytes (a local symbol), pinned SLUS_205.65.
 * Called by KbmReaderThread for each 28-byte record of the data reply whose halfword at +4
 * is 2. It uses the mouse slot table of GLibData (0x008ebdd8): count at +68, slot i at
 * 1224 + 104 * i with its identifier halfword at +4 of the slot. Nothing is returned.
 *   1. Nothing happens unless the count is positive and some slot's identifier equals the
 *      halfword at record +6 (signed compare, first match in order).
 *   2. Let flags be the halfword at slot +14 and p point at record +20. When bit 15 of
 *      flags is set, one byte is consumed from p and anything but 1 ends the call.
 *   3. When the word at record +12 is nonzero it is stored in the slot's word at +16 and
 *      the call ends.
 *   4. Otherwise it waits on the semaphore in the word at GLibData +44 and stores an
 *      8-byte entry in the slot's ring at slot + 64 + 8 * w, where w is the word at slot
 *      +36: word 0 of the record goes to entry bytes 0..3, then the next three bytes from
 *      p to bytes 4, 5 and 6, and when bit 14 of flags is set one more byte from p to
 *      byte 7. The write position becomes (w + 1) % 5 (signed remainder). If it now equals
 *      the word at slot +32 (the read position), that word becomes (itself + 1) % 5,
 *      dropping the oldest entry; otherwise the halfword at slot +60 is incremented. Last,
 *      it signals the same semaphore.
 * Inferred: a five-entry ring with write position at +36 and read position at +32, the
 * halfword at +60 counting queued entries, bit 15 a leading marker byte requirement and bit
 * 14 a fourth data byte. Unknown: what the record words mean beyond the offsets used, and
 * the meaning of the word at +16. WaitSema and SignalSema are the image's own.
 */
extern "C" {
extern unsigned char GLibData[];
int WaitSema(int semaphore);
int SignalSema(int semaphore);

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;

void process_mouse_data(void* data_record) {
    const unsigned char* const record = static_cast<const unsigned char*>(data_record);
    unsigned char* const g = GLibData;
    const int count = *reinterpret_cast<AliasedInt*>(g + 68);
    if (count <= 0) return;
    const unsigned char* source = record + 20;
    const short identifier = *reinterpret_cast<const AliasedShort*>(record + 6);
    int index = 0;
    while (*reinterpret_cast<AliasedShort*>(g + 1228 + 104 * index) != identifier) {
        ++index;
        if (!(index < count)) return;
    }
    if (!(index < *reinterpret_cast<AliasedInt*>(g + 68))) return;
    unsigned char* const slot = g + 1224 + 104 * index;
    const int position = *reinterpret_cast<AliasedInt*>(slot + 36);
    unsigned char* const entry = slot + position * 8 + 64;
    const unsigned flags = *reinterpret_cast<AliasedUShort*>(slot + 14);
    if (flags & 0x8000u) {
        if (*source++ != 1) return;
    }
    const int skipped = *reinterpret_cast<const AliasedInt*>(record + 12);
    if (skipped != 0) {
        *reinterpret_cast<AliasedInt*>(slot + 16) = skipped;
        return;
    }
    WaitSema(*reinterpret_cast<AliasedInt*>(g + 44));
    *reinterpret_cast<AliasedInt*>(entry) = *reinterpret_cast<const AliasedInt*>(record);
    entry[4] = *source++;
    entry[5] = *source++;
    entry[6] = *source++;
    if (*reinterpret_cast<AliasedUShort*>(slot + 14) & 0x4000u) entry[7] = *source;
    const int next = (*reinterpret_cast<AliasedInt*>(slot + 36) + 1) % 5;
    *reinterpret_cast<AliasedInt*>(slot + 36) = next;
    if (next == *reinterpret_cast<AliasedInt*>(slot + 32))
        *reinterpret_cast<AliasedInt*>(slot + 32) = (next + 1) % 5;
    else
        *reinterpret_cast<AliasedUShort*>(slot + 60) = *reinterpret_cast<AliasedUShort*>(slot + 60) + 1;
    SignalSema(*reinterpret_cast<AliasedInt*>(GLibData + 44));
}
}
