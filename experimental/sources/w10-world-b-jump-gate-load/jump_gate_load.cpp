// Observed jumpGateLoad__FPc at 0x002847f8, 52 bytes in SLUS_205.65. Reads the first word of
// the argument buffer, keeps its low byte in the byte jumpGateDestCount (0x00486da0), copies
// 480 bytes from buffer + 4 into jumpGateEntries (0x008d60e0, fifteen 32-byte names) with
// memcpy, and returns 484, the number of bytes consumed. It mirrors jumpGateSave__FPc, which
// writes the count word followed by the same 480 bytes.
// Unknown: whether the count is validated anywhere (it is not here).
extern "C" void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);
extern signed char jumpGateDestCount;
extern char jumpGateEntries[];

extern "C" int jumpGateLoad__FPc(char* buffer) {
    jumpGateDestCount = static_cast<signed char>(*reinterpret_cast<int*>(buffer));
    memcpy(jumpGateEntries, buffer + 4, 480);
    return 484;
}
