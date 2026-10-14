// Observed: unet_startup_thread__FPv at 0x0101c448 is 516 bytes; the argument is not used and
// nothing is returned. Steps, in order:
//  1. sceSifMInitRpc(0); VIInitUSocketExApi__Fv().
//  2. sceLibnetInitialize(cd, 2048, 8192, 32); sLibNetInitialized = 1 (stored before the next
//     call); net_buf = __wrap_memalign(64, 0x10200) (stored whether or not it is null).
//  3. net_buf null: pf("malloc failed.\n") and then an endless loop of nops.
//  4. sceLibnetRegisterHandler(cd, net_buf) negative: pf("reg_handler() failed.\n"), endless loop.
//  5. memset of a 16-byte stack record to zero; sceLibnetWaitGetAddress(cd, net_buf, second
//     16-byte stack record, 1, first record, 2) negative: printf("wait_get_addr() failed.\n"),
//     endless loop.
//  6. sceInetAddress2String(cd, net_buf, 128-byte stack text, 128, first record) nonzero:
//     printf("invalid addr\n"), endless loop.
//  7. sceInetAddress2Name(cd, net_buf, 0, hostname, 128, first record, 0, 0): nonzero result:
//     strcpy(hostname, text), printf("My address: %s\n", text); zero result:
//     printf("My address: %s (%s)\n", hostname, text). Then it returns.
// hostname (0x0118e6c0, 128 bytes), net_buf (0x0118e6b0) and cd (0x0118e740, 60 bytes) are
// image globals; unet_startup_async creates this thread and unet_startup_async_status ends it
// once its status reads 16.
// Inferred: this brings the network stack up and records the host name and printed address;
// the hang loops are the failure policy for allocation and address lookup. The nop count of
// the original loops (a few nops around the back branch) is not reproduced; the hang is.
// Unknown: the exact SDK signatures of the libnet and inet calls (the argument types here
// are only the observed register values), the meaning of the records at sp+0 and sp+16, what
// pf does beyond printing, and the three arguments after the host name in the name lookup.
#ifndef NORRATH_STARTUP_HANG
#define NORRATH_STARTUP_HANG() for (;;) {}
#endif
extern "C" {
extern int sLibNetInitialized;
extern void* net_buf;
extern unsigned char cd[];
extern char hostname[];
void sceSifMInitRpc(int mode);
void VIInitUSocketExApi__Fv();
int sceLibnetInitialize(void* client, int first, int second, int third);
void* __wrap_memalign(unsigned alignment, unsigned size);
void pf(const char* message);
int printf(const char* format, ...);
void* memset(void* destination, int value, unsigned length);
char* strcpy(char* destination, const char* source);
int sceLibnetRegisterHandler(void* client, void* buffer);
int sceLibnetWaitGetAddress(void* client, void* buffer, void* result, int first, void* address, int second);
int sceInetAddress2String(void* client, void* buffer, char* text, int length, void* address);
int sceInetAddress2Name(void* client, void* buffer, void* first, char* name, int length, void* address, int second, int third);

struct NetStartFrame {
    unsigned char address[16];
    unsigned char wait_result[16];
    char text[128];
};

void unet_startup_thread__FPv(void*) {
    NetStartFrame frame;
    sceSifMInitRpc(0);
    VIInitUSocketExApi__Fv();
    sceLibnetInitialize(cd, 2048, 8192, 32);
    sLibNetInitialized = 1;
    net_buf = __wrap_memalign(64, 0x10200);
    if (net_buf == nullptr) {
        pf("malloc failed.\n");
        NORRATH_STARTUP_HANG();
    }
    if (sceLibnetRegisterHandler(cd, net_buf) < 0) {
        pf("reg_handler() failed.\n");
        NORRATH_STARTUP_HANG();
    }
    memset(frame.address, 0, 16);
    if (sceLibnetWaitGetAddress(cd, net_buf, frame.wait_result, 1, frame.address, 2) < 0) {
        printf("wait_get_addr() failed.\n");
        NORRATH_STARTUP_HANG();
    }
    if (sceInetAddress2String(cd, net_buf, frame.text, 128, frame.address) != 0) {
        printf("invalid addr\n");
        NORRATH_STARTUP_HANG();
    }
    if (sceInetAddress2Name(cd, net_buf, nullptr, hostname, 128, frame.address, 0, 0) != 0) {
        strcpy(hostname, frame.text);
        printf("My address: %s\n", frame.text);
    } else {
        printf("My address: %s (%s)\n", hostname, frame.text);
    }
}
}
