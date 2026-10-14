// Observed: PlayBgm__13VISoundDeviceiPCc at 0x011264c0 is 44 bytes. Arguments: device (a0),
// an index (a1) and a C string (a2). It copies the string with strcpy to
// device + (index << 8) + 3436, so each slot is 256 bytes apart, and returns 0.
// Inferred: a table of background-music name buffers starting at +3436, 256 bytes per
// slot; the name is stored, not played, by this function. The 256-byte slot size is the
// stride only; the copy is not bounded here.
// Unknown: the number of slots, and who consumes the stored name.
extern "C" {
char* strcpy(char* dst, const char* src);

int PlayBgm__13VISoundDeviceiPCc(unsigned char* device, int slot, const char* name) {
    strcpy(reinterpret_cast<char*>(device + (slot << 8) + 3436), name);
    return 0;
}
}
