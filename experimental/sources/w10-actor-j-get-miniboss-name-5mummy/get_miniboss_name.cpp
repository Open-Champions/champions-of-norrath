// Observed getMinibossName__5Mummy at 0x0023be78, 60 bytes. Formats the signed byte at
// object+76 with the format text "MBMummy%d" into a static buffer (the bss block at 0x007d7e20,
// a local of this function in the executable) and returns the buffer. Nothing here ran on
// the EE.
extern "C" {
extern char mummy_miniboss_name_buffer_7d7e20[];
int sprintf(char* to, const char* format, ...);

char* getMinibossName__5Mummy(void* self) {
    sprintf(mummy_miniboss_name_buffer_7d7e20, "MBMummy%d", static_cast<signed char*>(self)[76]);
    return mummy_miniboss_name_buffer_7d7e20;
}
}
