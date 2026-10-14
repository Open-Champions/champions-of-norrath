// Observed CheckNetwork__15VINetworkStatus at 0x010a3950, 184 bytes. It
// returns an int. A zero word at offset 60 returns 2. A count at offset
// 104 that is not positive returns 1. Otherwise each 4-byte id at offset
// 64 is passed to sceInetCtlGetState with the object, the word at offset
// 60, and a stack state preset to 0. The count is read again after each
// call. A zero call result whose state is 3 returns 0. Every other result
// continues. If none of those states is 3, the function returns 1.
extern "C" {
struct VINetworkStatus;
int printf(const char* text, ...);
int sceInetCtlGetState(VINetworkStatus* object, void* buffer, int id, int* state);

int CheckNetwork__15VINetworkStatus(VINetworkStatus* object) {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(object);
    if (*reinterpret_cast<unsigned*>(bytes + 60) == 0) return 2;
    int count = *reinterpret_cast<int*>(bytes + 104);
    if (count <= 0) return 1;
    int index = 0;
    while (index < count) {
        printf("whob?\n");
        int state = 0;
        unsigned buffer_word = *reinterpret_cast<unsigned*>(bytes + 60);
        int id = *reinterpret_cast<int*>(bytes + 64 + static_cast<unsigned>(index) * 4u);
        int result = sceInetCtlGetState(
            object, reinterpret_cast<void*>(static_cast<unsigned long>(buffer_word)), id, &state);
        if (result == 0 && state == 3) return 0;
        index += 1;
        count = *reinterpret_cast<int*>(bytes + 104);
    }
    return 1;
}
}
