// Observed keyboardFree__Fv at 0x002cb228, 32 bytes. It passes the local
// object vik at 0x008d86a0 to Free__10VIKeyboard and returns. vik is .bss
// with st_size 0 and local binding. The next symbol, outMsg, is 536 bytes
// later. Init__10VIKeyboard stores a word at offset 528, so the used prefix
// is at least 532 bytes. Bytes 532 through 535 are only the gap to outMsg.
// This function does not read the object. What Free does is unknown.
extern "C" void Free__10VIKeyboard(void* object);
extern "C" void Init__10VIKeyboard(void* object);

static unsigned char vik[536];

extern "C" void keyboardFree__Fv() {
    Free__10VIKeyboard(vik);
}

// Observed keyboardRestart__Fv at 0x002cb248, 32 bytes. It passes the same
// vik object to Init__10VIKeyboard and returns. It does not read the object.
extern "C" void keyboardRestart__Fv() {
    Init__10VIKeyboard(vik);
}
