// Observed msg_save__8JumpGatePc at 0x00284538, 36 bytes.
// Writes two bytes to the buffer: the byte at object+77, then the byte at object+78. Returns the
// number of bytes written, always 2. Nothing else is read or written.
// Unknown: what the two bytes mean. Nothing here ran on the EE.
extern "C" int msg_save__8JumpGatePc(void* self, char* out) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    char* cursor = out;
    *cursor++ = static_cast<char>(object[77]);
    *cursor++ = static_cast<char>(object[78]);
    return static_cast<int>(cursor - out);
}
