// Observed Initialize__15VINetworkStatus at 0x010a3788, 204 bytes. It
// returns an int. A nonzero sceLibnetInitialize result returns -1.
// Otherwise a word already at offset 60 is freed, memalign(64, 2048)
// replaces it, and "bb\n" is printed. A null allocation returns -1.
// Otherwise "cc\n" is printed and sceInetGetInterfaceList is called with
// the object, that allocation, offset 64, and 10. The result is stored at
// offset 104 before "dd\n". A negative result is then replaced with 0 and
// this function returns -1. Any other result returns 0.
extern "C" {
struct VINetworkStatus;
int printf(const char* text, ...);
int sceLibnetInitialize(VINetworkStatus* object, int first, int second, int third);
void __wrap_free(void* pointer);
void* __wrap_memalign(unsigned alignment, unsigned size);
int sceInetGetInterfaceList(VINetworkStatus* object, void* buffer, void* list, int count);

int Initialize__15VINetworkStatus(VINetworkStatus* object) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    printf("aa\n");
    if (sceLibnetInitialize(object, 2048, 8192, 32) != 0) return -1;
    unsigned previous_word = *reinterpret_cast<unsigned*>(bytes + 60);
    if (previous_word != 0) __wrap_free(reinterpret_cast<void*>(static_cast<unsigned long>(previous_word)));
    void* allocation = __wrap_memalign(64, 2048);
    *reinterpret_cast<unsigned*>(bytes + 60) =
        static_cast<unsigned>(reinterpret_cast<unsigned long>(allocation));
    printf("bb\n");
    if (allocation == nullptr) return -1;
    printf("cc\n");
    int listed = sceInetGetInterfaceList(object, allocation, bytes + 64, 10);
    *reinterpret_cast<int*>(bytes + 104) = listed;
    printf("dd\n");
    if (listed < 0) {
        *reinterpret_cast<int*>(bytes + 104) = 0;
        return -1;
    }
    return 0;
}
}
