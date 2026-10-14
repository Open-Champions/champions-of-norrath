// Observed msg_save__5TorchPc at 0x00231008, 24 bytes. Stores the signed byte at object+138 as a
// 32-bit word at the start of the buffer and returns 4, the number of bytes written. Nothing else is
// read or written.
// Unknown: what the byte at +138 means (0, 1, 2 and 3 are the values other Torch code compares
// against). Nothing here ran on the EE.
extern "C" int msg_save__5TorchPc(void* self, char* buffer) {
    const signed char* object = static_cast<const signed char*>(self);
    *reinterpret_cast<int*>(buffer) = object[138];
    return 4;
}
