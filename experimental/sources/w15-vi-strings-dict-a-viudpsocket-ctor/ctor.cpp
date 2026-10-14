// Observed: __11VIUDPSocketi at 0x0102a178 is 72 bytes. Arguments: the object (a0) and an int (a1).
// Stores a1 at offset 0 and -1 at offset 68, calls __wrap_memalign(64, 20480) and stores the
// result at offset 4, then tail-calls sceLibnetInitialize(object + 8, 2048, 8192, 32). The value
// that final call returns is left in the return register unchanged; the constructor does not
// return the object pointer.
// Inferred: a UDP socket wrapper that takes an int (likely a port or handle) and allocates a
// 20480-byte 64-aligned buffer; object+8 is a library network-init area; object+68 a handle slot
// initialised to -1.
// Unknown: the meaning of the int, the library call's parameter types (the third and fourth are
// passed as plain integers) and the object size.
extern "C" {
void* __wrap_memalign(unsigned int alignment, unsigned int size);
int sceLibnetInitialize(void* area, int a, int b, int c);

void __11VIUDPSocketi(unsigned char* self, int value) {
    *reinterpret_cast<int*>(self) = value;
    *reinterpret_cast<int*>(self + 68) = -1;
    *reinterpret_cast<void**>(self + 4) = __wrap_memalign(64, 20480);
    sceLibnetInitialize(self + 8, 2048, 8192, 32);
}
}
