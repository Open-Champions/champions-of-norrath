// Observed msg_load__5TorchPc at 0x00231020, 72 bytes. Reads one 32-bit word from the buffer. Its low
// byte goes to object+138. The flag word at object+12 first has bits 0x201 cleared, then bit 0x200
// is set when the low byte of the loaded word is nonzero, and bit 0x1 otherwise. Returns 4, the
// number of bytes consumed. The image stores the cleared word before the final one; both stores are
// kept in that order.
// Unknown: what flag bits 0x1 and 0x200 mean. Nothing here ran on the EE.
extern "C" int msg_load__5TorchPc(void* self, char* buffer) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned* flags = reinterpret_cast<unsigned*>(object + 12);
    const unsigned word = *reinterpret_cast<const unsigned*>(buffer);
    const unsigned cleared = *flags & ~0x201u;
    object[138] = static_cast<unsigned char>(word);
    *flags = cleared;
    if ((word & 0xffu) != 0) {
        *flags = cleared | 0x200u;
    } else {
        *flags = cleared | 0x1u;
    }
    return 4;
}
