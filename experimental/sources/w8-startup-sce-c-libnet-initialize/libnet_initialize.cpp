// Observed sceLibnetInitialize at 0x002e5d60, 200 bytes. The second argument
// must be at least 256 and a multiple of 64, otherwise -543 is returned. It
// then calls sceSifMBindRpcParam on the first argument with id 0x80001201, a
// zero third argument and the remaining three arguments. A negative result
// returns -540. The word at offset 36 of the first argument is then examined;
// zero means the bind is not finished, so a counted empty delay of 65537
// iterations runs and the bind is issued again. A nonzero word returns 0.
extern "C" {
int sceSifMBindRpcParam(void* client, unsigned id, unsigned mode, int first, int second, int third);
int sceLibnetInitialize(void* client, int first, int second, int third) {
    if (first < 256 || first != ((first + 63) & -64)) return -543;
    for (;;) {
        if (sceSifMBindRpcParam(client, 0x80001201u, 0, first, second, third) < 0) return -540;
        if (reinterpret_cast<int*>(client)[9] != 0) return 0;
        for (int count = 0x10000; count != -1; --count) asm volatile("");
    }
}
}
