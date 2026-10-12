// Observed initMiniboss__8Creature at 0x00112940, 12 bytes.
// Stores byte 1 at creature+76 and returns 1. No other byte is written.
extern "C" int initMiniboss__8Creature(void* self) {
    static_cast<unsigned char*>(self)[76] = 1;
    return 1;
}
