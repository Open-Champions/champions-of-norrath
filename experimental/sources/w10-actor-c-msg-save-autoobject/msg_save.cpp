// Observed msg_save__10AutoObjectPc at 0x00224aa8, 8 bytes: returns 0.
// It writes nothing to the buffer and does not read the object; the caller treats the return
// value as the number of bytes written (inferred from the sibling save methods, which return
// the buffer advance).
extern "C" int msg_save__10AutoObjectPc(void*, char*) { return 0; }
