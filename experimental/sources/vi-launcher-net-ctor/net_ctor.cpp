// Observed __15VINetworkStatus at 0x010a3858, 68 bytes. It returns.
// The first 60 bytes, the word at offset 60, the 40 bytes at offset 64,
// and the word at offset 104 are cleared. The full object size is unknown.
extern "C" {
struct VINetworkStatus;
void* memset(void* destination, int value, unsigned count);

void __15VINetworkStatus(VINetworkStatus* object) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    memset(object, 0, 60);
    *reinterpret_cast<int*>(bytes + 60) = 0;
    memset(bytes + 64, 0, 40);
    *reinterpret_cast<int*>(bytes + 104) = 0;
}
}
