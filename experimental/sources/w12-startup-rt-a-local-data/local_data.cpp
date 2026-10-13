// Observed LocalData, a local zero-initialised (bss) object of the keyboard/mouse library at
// 0x008e8c80, pinned SLUS_205.65. The symbol table gives no size; the next object, object.2, starts at
// 0x008ea980, so the extent is 0x1d00 = 7424 bytes. That extent is a gap measurement, not proof the
// whole range is one object; the code in KbmReaderThread and InitialiseIopDriver uses bytes 0..720.
// Used offsets (bytes): +0..+3 two words of reply header, +4.. device records 28 bytes apart (see
// KbmReaderThread), +512..+659 the poll request/reply buffer, +704..+719 the driver-version packet
// of InitialiseIopDriver (halfword, halfword, halfword, halfword, word).
// Alignment 64 is chosen because the buffer is handed to sceSifCallRpc as send and receive area; the
// observed address is also a multiple of 128. That requirement is inferred, not observed.
extern "C" {
unsigned char LocalData[0x1d00] __attribute__((aligned(64)));
}
