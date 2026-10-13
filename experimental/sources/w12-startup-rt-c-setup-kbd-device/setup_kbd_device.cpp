/* Observed setup_kbd_device at 0x002e7ce8, 336 bytes (a local symbol), pinned SLUS_205.65.
 * Called by KbmReaderThread for a 16-byte device record whose halfword at +4 is an
 * identifier. It works on the keyboard slot table of GLibData (0x008ebdd8): the word at +64
 * is the slot count and slot i starts at byte 136 + 272 * i, so that its identifier
 * halfword sits at 140 + 272 * i (the reader thread sets every one to -1 at start-up).
 *   1. If the slot count is not positive, or some slot already holds the record's
 *      identifier (compared as signed halfwords), it returns 1 and changes nothing.
 *   2. Otherwise it takes the first slot whose identifier halfword is -1; none free gives 1.
 *   3. A free slot gets the 16 bytes of the record at slot +0, zeros in the word at +256,
 *      eight bytes from +260, the halfwords at +36 and +38 and thirty-two bytes from +224,
 *      the words at +16 and +28 become 0 and 1, and the word at GLibData +100 becomes 1.
 *      The result is 0.
 * Inferred: the word at GLibData +100 is a "device list changed" flag, the word at slot +28
 * an in-use flag; the zeroed ranges are per-device state. Unknown: the meaning of those
 * fields and of the 16 record bytes beyond the identifier. memset is the image's own.
 */
extern "C" {
extern unsigned char GLibData[];
void* memset(void* destination, int value, unsigned long size);

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

int setup_kbd_device(void* device_record) {
    const unsigned char* const record = static_cast<const unsigned char*>(device_record);
    unsigned char* const g = GLibData;
    const int count = *reinterpret_cast<AliasedInt*>(g + 64);
    if (count <= 0) return 1;
    const short identifier = *reinterpret_cast<const AliasedShort*>(record + 4);
    for (int i = 0; i < count; ++i)
        if (*reinterpret_cast<AliasedShort*>(g + 140 + 272 * i) == identifier) return 1;
    int index = 0;
    while (*reinterpret_cast<AliasedShort*>(g + 140 + 272 * index) != -1) {
        ++index;
        if (!(index < count)) return 1;
    }
    unsigned char* const slot = g + 136 + 272 * index;
    __builtin_memcpy(slot, record, 16);
    *reinterpret_cast<AliasedInt*>(slot + 256) = 0;
    memset(slot + 260, 0, 8);
    *reinterpret_cast<AliasedShort*>(slot + 38) = 0;
    *reinterpret_cast<AliasedShort*>(slot + 36) = 0;
    memset(slot + 224, 0, 32);
    *reinterpret_cast<AliasedInt*>(slot + 16) = 0;
    *reinterpret_cast<AliasedInt*>(slot + 28) = 1;
    *reinterpret_cast<AliasedInt*>(g + 100) = 1;
    return 0;
}
}
