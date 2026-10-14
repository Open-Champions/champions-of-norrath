// Observed msg_load__11FloorSwitchPc at 0x21c120, 24 bytes. Reads one byte from the
// stream, stores it at object+80 and returns the number of bytes consumed, 1.
// The counterpart of msg_save__11FloorSwitchPc, which writes that byte.
extern "C" int msg_load__11FloorSwitchPc(void* self, const char* in) {
    static_cast<char*>(self)[80] = in[0];
    return 1;
}
