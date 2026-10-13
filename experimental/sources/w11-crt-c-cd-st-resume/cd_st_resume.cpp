// Observed sceCdStResume at 0x002e43b8, 84 bytes. It stores 1 in stm_status
// (0x00447c34) unconditionally; the store is the delay slot of the debug test,
// so it happens before the optional print. When the signed SCE_CD_debug word
// (0x00446090) is above zero it prints "sceCdStResume call\n". It returns
// sceCdStream(0, 0, 0, 8, dum_mode); dum_mode is the 40-byte object at
// 0x008e83d8.
// Inferred: command 8 resumes the stream, following the pause routine's use of 7.
extern "C" {
extern int SCE_CD_debug;
extern int stm_status;
extern unsigned char dum_mode[];
int norrath_sce_printf(const char* format, ...);
int sceCdStream(int argument, int count, void* buffer, int command, void* mode);

int sceCdStResume() {
    stm_status = 1;
    if (SCE_CD_debug > 0) norrath_sce_printf("sceCdStResume call\n");
    return sceCdStream(0, 0, 0, 8, dum_mode);
}
}
