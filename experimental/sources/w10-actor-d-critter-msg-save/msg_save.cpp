// Observed msg_save__7CritterPc at 0x001f1900, 8 bytes.
// Returns zero without touching the object or the buffer. Unknown: why the slot has an empty body;
// the byte count of zero means Critter adds nothing to the Creature save data, which is inferred
// only from the return value. Nothing here ran on the EE.
extern "C" int msg_save__7CritterPc(void* self, char* buffer) {
    (void)self; (void)buffer;
    return 0;
}
