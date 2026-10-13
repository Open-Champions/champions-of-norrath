/* Observed setup_mouse_device at 0x002e7e38, 288 bytes (a local symbol), pinned SLUS_205.65.
 * Called by KbmReaderThread for a 16-byte device record whose halfword at +4 is an
 * identifier. It works on the mouse slot table of GLibData (0x008ebdd8): the word at +68
 * is the slot count and slot i starts at byte 1224 + 104 * i, so that its identifier
 * halfword sits at 1228 + 104 * i (the reader thread sets every one to -1 at start-up).
 *   1. If the slot count is not positive, or some slot already holds the record's
 *      identifier (compared as signed halfwords), it returns 1 and changes nothing.
 *   2. Otherwise it takes the first slot whose identifier halfword is -1; none free gives 1.
 *   3. A free slot gets the 16 bytes of the record at slot +0, the words at +16, +32 and
 *      +36 and the halfword at +60 cleared, the word at +28 set to 1, and the word at
 *      GLibData +104 set to 1. The result is 0.
 * It is the mouse counterpart of setup_kbd_device, with a smaller slot and fewer cleared
 * fields. Inferred: the word at GLibData +104 is a "device list changed" flag next to the
 * keyboard's flag at +100, and the word at slot +28 an in-use flag.
 * Unknown: the meaning of those fields and of the record bytes beyond the identifier.
 */
extern "C" {
extern unsigned char GLibData[];

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;

int setup_mouse_device(void* device_record) {
    const unsigned char* const record = static_cast<const unsigned char*>(device_record);
    unsigned char* const g = GLibData;
    const int count = *reinterpret_cast<AliasedInt*>(g + 68);
    if (count <= 0) return 1;
    const short identifier = *reinterpret_cast<const AliasedShort*>(record + 4);
    for (int i = 0; i < count; ++i)
        if (*reinterpret_cast<AliasedShort*>(g + 1228 + 104 * i) == identifier) return 1;
    int index = 0;
    while (*reinterpret_cast<AliasedShort*>(g + 1228 + 104 * index) != -1) {
        ++index;
        if (!(index < count)) return 1;
    }
    unsigned char* const slot = g + 1224 + 104 * index;
    __builtin_memcpy(slot, record, 16);
    *reinterpret_cast<AliasedInt*>(slot + 28) = 1;
    *reinterpret_cast<AliasedInt*>(slot + 16) = 0;
    *reinterpret_cast<AliasedInt*>(g + 104) = 1;
    *reinterpret_cast<AliasedShort*>(slot + 60) = 0;
    *reinterpret_cast<AliasedInt*>(slot + 32) = 0;
    *reinterpret_cast<AliasedInt*>(slot + 36) = 0;
    return 0;
}
}
