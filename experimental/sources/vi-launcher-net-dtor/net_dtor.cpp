// Observed _$_15VINetworkStatus at 0x010a38a0, 172 bytes. It returns.
// A nonzero word at offset 60 is freed, cleared, then followed by "a1\n",
// sceLibnetTerminate, and "a2\n". The first 60 bytes and the 40 bytes at
// offset 64 are then cleared, and offset 104 is cleared. Bit 0 of the flag
// selects __builtin_delete. What terminate releases is unknown.
extern "C" {
struct VINetworkStatus;
void __wrap_free(void* pointer);
int printf(const char* text, ...);
void sceLibnetTerminate(VINetworkStatus* object);
void* memset(void* destination, int value, unsigned count);
void __builtin_delete(void* pointer);

void _$_15VINetworkStatus(VINetworkStatus* object, int flags) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    unsigned buffer_word = *reinterpret_cast<unsigned*>(bytes + 60);
    if (buffer_word != 0) {
        __wrap_free(reinterpret_cast<void*>(static_cast<unsigned long>(buffer_word)));
        *reinterpret_cast<unsigned*>(bytes + 60) = 0;
        printf("a1\n");
        sceLibnetTerminate(object);
        printf("a2\n");
    }
    memset(object, 0, 60);
    memset(bytes + 64, 0, 40);
    *reinterpret_cast<int*>(bytes + 104) = 0;
    if ((flags & 1) != 0) __builtin_delete(object);
}
}
