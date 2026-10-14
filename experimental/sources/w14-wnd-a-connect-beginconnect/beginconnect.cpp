// Observed: BeginConnect__12VIWndConnect at 0x010a22a8 is 824 bytes. Words of this (by byte offset): +1916 an index i,
// +1972 a pointer to an array of 620-byte records, +1928, +1936, +1940, +1944, +1948 five signed module handles
// (called m0 = +1928, m1 = +1936, m2 = +1940, m3 = +1944, m4 = +1948 below). It copies 104 bytes from record i at
// offset 516 into a local c, where the word c+4 is a kind and the bytes at c+72 a short text; sce is true when the kind
// is 2 or 3 and strncmp(c+72, "SCE", 3) == 0. Then, in order:
//  1. when sce is false and m0 >= 0: printf("unloading smap\n"), VIEEUnloadIrx__Fi(m0), m0 = -1;
//  2. when m4 >= 0: printf("unloading spduart\n"), VIEEUnloadIrx__Fi(m4), m4 = -1;
//  3. when the kind is not 2 and m3 >= 0: printf("unloading pppoe\n"), VIEEUnloadIrx__Fi(m3), m3 = -1;
//  4. sceGsSyncV(0) four times;
//  5. kind 2: when m2 < 0, m2 = VIEELoadIrx__FPCciT0("PPP.IRX", 0, 0); then when m3 < 0, m3 = VIEELoadIrx("PPPOE.IRX",
//     0, 0). Kind 3 skips this. Any other kind returns -1 here, leaving the loads undone;
//  6. when strncmp(c+72, "SCE", 3) == 0: when m0 < 0, build "cdrom0:\MODULES\SMAP.IRX;1" with snprintf__8VIStringPciPCce
//     into a 256-byte buffer, printf("%s\n", buffer), then up to 26 attempts of sceSifLoadModule(buffer, 0, 0) (the
//     retry counter starts at 25, is tested after each failure and decremented with the message "sceSifLoadModule:
//     cannot load '%s'\n"), and m0 = max(result, -1). Otherwise, when m1 < 0, m1 = VIEELoadIrx("AN986.IRX", 0, 0);
//  7. the result is that of BringUpNetwork__12VIWndConnect(this).
// Inferred: this is the network-adapter setup for the online connect window: the SCE text selects the PlayStation 2
// network adapter modules (SMAP) and anything else the AN986 driver; PPP/PPPoE are loaded for the dial-up kind. The
// strings are in the original's read-only data and are reproduced here as text.
// Unknown: the record layout and the meaning of the kinds and handles, and the declared type of the first printf
// argument of the retry message (the original passes the buffer and the failed result; only the buffer is used).
extern "C" {
int strncmp(const char* a, const char* b, unsigned n);
int printf(const char* format, ...);
int snprintf__8VIStringPciPCce(char* buffer, int size, const char* format, ...);
int sceGsSyncV(int mode);
int sceSifLoadModule(const char* path, int args, const char* argp);
int VIEELoadIrx__FPCciT0(const char* name, int a, int b);
void VIEEUnloadIrx__Fi(int handle);
int BringUpNetwork__12VIWndConnect(void* self);

struct ConnectInfo {
    unsigned char bytes[104];
};

int BeginConnect__12VIWndConnect(void* self) {
    int* w = static_cast<int*>(self);
    const unsigned char* records = reinterpret_cast<const unsigned char*>(static_cast<unsigned long>(static_cast<unsigned>(w[493])));
    const ConnectInfo info = *reinterpret_cast<const ConnectInfo*>(records + w[479] * 620 + 516);
    const int kind = *reinterpret_cast<const int*>(info.bytes + 4);
    const char* name = reinterpret_cast<const char*>(info.bytes + 72);
    int sce = 0;
    if (kind == 2 || kind == 3) sce = strncmp(name, "SCE", 3) == 0;
    if (!sce) {
        if (w[482] >= 0) {
            printf("unloading smap\n");
            VIEEUnloadIrx__Fi(w[482]);
            w[482] = -1;
        }
    }
    if (w[487] >= 0) {
        printf("unloading spduart\n");
        VIEEUnloadIrx__Fi(w[487]);
        w[487] = -1;
    }
    if (kind != 2 && w[486] >= 0) {
        printf("unloading pppoe\n");
        VIEEUnloadIrx__Fi(w[486]);
        w[486] = -1;
    }
    sceGsSyncV(0);
    sceGsSyncV(0);
    sceGsSyncV(0);
    sceGsSyncV(0);
    if (kind == 2) {
        if (w[485] < 0) w[485] = VIEELoadIrx__FPCciT0("PPP.IRX", 0, 0);
        if (w[486] < 0) w[486] = VIEELoadIrx__FPCciT0("PPPOE.IRX", 0, 0);
    } else if (kind != 3) {
        return -1;
    }
    if (strncmp(name, "SCE", 3) == 0) {
        if (w[482] < 0) {
            char path[256];
            snprintf__8VIStringPciPCce(path, 256, "cdrom0:\\MODULES\\%s;1", "SMAP.IRX");
            int retries = 25;
            printf("%s\n", path);
            int result;
            for (;;) {
                result = sceSifLoadModule(path, 0, 0);
                if (result >= 0 || retries <= 0) break;
                printf("sceSifLoadModule: cannot load '%s'\n", path, result);
                --retries;
            }
            w[482] = result < -1 ? -1 : result;
        }
    } else if (w[484] < 0) {
        w[484] = VIEELoadIrx__FPCciT0("AN986.IRX", 0, 0);
    }
    return BringUpNetwork__12VIWndConnect(self);
}
}
