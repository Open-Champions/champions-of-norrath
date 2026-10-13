// Observed msg_save__11FloorSwitchPc at 0x21c108, 24 bytes. Writes the byte at
// object+80 to the output pointer and returns the number of bytes written, 1.
// Unknown: the meaning of that byte.
extern "C" int msg_save__11FloorSwitchPc(void* self, char* out) {
    *out = static_cast<char>(static_cast<const unsigned char*>(self)[80]);
    return 1;
}
