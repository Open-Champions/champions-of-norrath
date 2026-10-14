// Observed storage of the text-lookup code (textAddText and textGetString), with
// the address and size seen in the image. The image gives these no symbol, so the
// names below are authored here.
//   norrath_text_hash_slots      0x006d8fa0  32768 bytes, .bss: 8192 pointer-sized slots
//                                (open-addressed, slot = hash & 0x1fff)
//   norrath_text_missing_entries 0x006e0fa0    640 bytes, .bss: 16 entries of 20 halfwords
//   norrath_text_missing_cursor  0x00484038      1 byte (the original local is named unknownSpinner)
// The element types follow the callers (textAddText, textGetString). The pointer
// slots are written as char* and are 4 bytes wide on the target.
// Unknown: whether the original hash table was this exact size beyond the callers'
// mask of 0x1fff.
extern "C" {
char* norrath_text_hash_slots[8192];
unsigned short norrath_text_missing_entries[16][20];
unsigned char norrath_text_missing_cursor;
}
