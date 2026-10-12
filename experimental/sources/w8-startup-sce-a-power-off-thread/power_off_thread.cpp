/* Observed PowerOffThread__FPv at 0x002cc128, 180 bytes. It ignores its
 * argument. It waits on the semaphore whose id sits in sid (0x00486db4, the
 * word PreparePowerOff__Fv stores), stores 1 in powerOffMode (0x004865f8, one
 * byte), prints "power off request has come.\n" through scePrintf, calls
 * netFree__Fb(true), and prints "dev9 go down!\n" through printf. It then
 * repeats sceDevctl("dev9x:", 0x4402, 0, 0, 0, 0) until the result is not
 * negative (nothing is printed per retry), prints "dev9 down\n", and repeats
 * sceCdPowerOff on a local word until the call returns nonzero and the local
 * word is zero. It prints "power off\n" and spins in a loop of nops that never
 * exits. The store to powerOffMode sits in the delay slot of the scePrintf
 * call, so it happens before scePrintf starts. What 0x4402 or the sceCdPowerOff
 * result word mean is unknown. Compiled output is not claimed to match the
 * original.
 */
extern "C" {
extern int sid;
extern unsigned char powerOffMode;
int WaitSema(int semaphore);
int scePrintf(const char* format, ...);
int printf(const char* format, ...);
void netFree__Fb(bool flag);
int sceDevctl(const char* device, int command, void* input, int input_size, void* output, int output_size);
int sceCdPowerOff(int* result);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" bool norrath_host_keep_spinning();
#define NORRATH_SPIN() while (norrath_host_keep_spinning()) {}
#else
#define NORRATH_SPIN()                      \
    for (;;) {                              \
        asm volatile("nop; nop; nop; nop; nop" ::: "memory"); \
    }
#endif

extern "C" void PowerOffThread__FPv(void*) {
    WaitSema(sid);
    powerOffMode = 1;
    scePrintf("power off request has come.\n");
    netFree__Fb(true);
    printf("dev9 go down!\n");
    while (sceDevctl("dev9x:", 0x4402, nullptr, 0, nullptr, 0) < 0) {
    }
    printf("dev9 down\n");
    int result;
    do {
        while (sceCdPowerOff(&result) == 0) {
        }
    } while (result != 0);
    printf("power off\n");
    NORRATH_SPIN();
}
