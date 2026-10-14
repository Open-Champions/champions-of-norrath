// Observed gsm_create at 0x002eb790, 76 bytes. It requests 2568 (0xa08) bytes
// from __wrap_malloc. A null result is returned as null. Otherwise all 2568
// bytes are cleared with memset, the 64-bit doubleword at offset 2472 is set to
// 40, and the block is returned.
// Inferred, not proven: the name, the 2568-byte object, and the option field of
// gsm_option match the public GSM 06.10 library (a codec state with a
// "next residual pointer" initialised to 40), but the image carries no
// field names for this object.
extern "C" {
void* __wrap_malloc(unsigned size);
void* memset(void* destination, int value, unsigned count);

void* gsm_create() {
    unsigned char* state = static_cast<unsigned char*>(__wrap_malloc(2568u));
    if (state != 0) {
        memset(state, 0, 2568u);
        *reinterpret_cast<long long*>(state + 2472) = 40;
    }
    return state;
}
}
