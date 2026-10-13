// Observed msg_save__7ShooterPc at 0x001e5d80, 24 bytes: stores the unsigned halfword at object+82 as a
// 16-bit value at the start of the buffer and returns 2, the number of bytes written. Nothing else is
// read or written. Unknown: what the halfword at +82 holds. Nothing here ran on the EE.
extern "C" int msg_save__7ShooterPc(void* self, char* buffer) {
    *reinterpret_cast<unsigned short*>(buffer) = *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(self) + 82);
    return 2;
}
