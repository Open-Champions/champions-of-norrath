// Observed engineFrameEnd__Fb at 0x00190a48, 732 bytes.
// simpleDrawLoop calls it with 0 after increasing frameNm. This body is the
// call and store sequence from that extent. Fresh compilation is not an
// original-byte match. No EE path has run.
//
// GP is 0x004879f0. The named words below are the GP-relative targets.
// 0x01fc0000 is the address of _stack. The scan compares 5120 bytes with
// 0xA5 and, on a mismatch, calls scePrintf with the unsymbolized text at
// 0x00464328 ("stack error!  %s:%d\n"), the unsymbolized text at 0x00464268
// ("engine.cc"), and 1245, then stores zero at address 0 and continues.
// coreLoadingScreenRunning is the signed byte at 0x004866cc. A nonzero byte
// skips that scan.
//
// 0x10000000 is read and then cleared. Naming it as the EE timer count is
// inferred from the public timer map. The immediates 9766, 9767, 29299,
// and 4884 are the values in the instructions. 9766 * 3 is 29298; the
// compare is the immediate 29299, not a derived expression.
//
// `draw` is the BSS symbol at 0x00790ff0. Its ELF size is 0. The next
// symbol, dispEnv, is 64 bytes later. This function uses byte offsets
// 0 through 60. It does not establish a size.
//
// powerOffMode is the signed byte at 0x004865f8. Nonzero stores 1000 in
// engineDisplayDisable and does not reach the return: it reloads
// vblankCount, waits on vblankSemaphore2 while that word is unchanged,
// loads vblankField, and repeats.

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned char norrath_host_load8(unsigned address);
extern "C" unsigned norrath_host_load32(unsigned address);
extern "C" void norrath_host_store32(unsigned address, unsigned value);
#define NORRATH_LOAD8(address) norrath_host_load8(address)
#define NORRATH_LOAD32(address) norrath_host_load32(address)
#define NORRATH_STORE32(address, value) norrath_host_store32((address), (value))
#else
#define NORRATH_LOAD8(address) (*reinterpret_cast<volatile unsigned char*>(static_cast<unsigned>(address)))
#define NORRATH_LOAD32(address) (*reinterpret_cast<volatile unsigned*>(static_cast<unsigned>(address)))
#define NORRATH_STORE32(address, value) (*reinterpret_cast<volatile unsigned*>(static_cast<unsigned>(address)) = (value))
#endif

extern "C" {
extern volatile signed char coreLoadingScreenRunning;
extern volatile unsigned drawFrameNm;
extern int fullFrameMode;
extern unsigned switchToHalfCounter_797 asm("switchToHalfCounter.797");
extern int vblankSemaphore;
extern int switchTo30Counter_798 asm("switchTo30Counter.798");
extern int engineDisplayDisable;
extern unsigned lastFrameVBlankCount;
extern volatile signed char powerOffMode;
extern volatile int vblankCount;
extern volatile unsigned char vblankField;
extern int vblankSemaphore2;
extern unsigned draw[];

void scePrintf(const char* text, const char* site, int line);
void drawWaitFinished__Fv();
void sceGsSyncPath(int mode, int timeout);
void texturePackDown__Fv();
void engineSetVideoMode__Fb(int mode);
void WaitSema(int id);
int PollSema(int id);
int DIntr();
void EIntr();
void doSuperSample__Fiib(int selector, int word, int argument);

void engineFrameEnd__Fb(int argument) {
    if (coreLoadingScreenRunning == 0) {
        unsigned index = 0;
        while (index < 5120u) {
            const unsigned char byte = NORRATH_LOAD8(0x01fc0000u + index);
            index += 1u;
            if (byte != 0xa5u) {
                scePrintf("stack error!  %s:%d\n", "engine.cc", 1245);
#if defined(NORRATH_HOST_CONTRACT)
                NORRATH_STORE32(0u, 0u);
#else
                // sw zero, 0(zero). The instruction falls through. A C store
                // of address 0 is deleted under the reviewed flags.
                asm volatile("sw $0, 0($0)" ::: "memory");
#endif
            }
        }
    }

    drawWaitFinished__Fv();
    sceGsSyncPath(0, 0);
    drawFrameNm = drawFrameNm + 1u;
    texturePackDown__Fv();

    if (fullFrameMode == 0) {
        switchToHalfCounter_797 = 0u;
    } else {
        const unsigned timer = NORRATH_LOAD32(0x10000000u);
        const bool timer_below = (timer & 0x80000000u) != 0u || timer < 9767u;
        if (!timer_below) {
            switchToHalfCounter_797 = 0u;
        } else {
            const unsigned next = switchToHalfCounter_797 - timer + 9766u;
            switchToHalfCounter_797 = next;
            const bool next_below = (next & 0x80000000u) != 0u || next < 29299u;
            if (!next_below) engineSetVideoMode__Fb(0);
        }
    }

    const unsigned gate_offset = 28u + (draw[15] << 4);
    if (draw[gate_offset / 4u] != 0u) WaitSema(vblankSemaphore);
    int polled = PollSema(vblankSemaphore);
    for (;;) {
        const int current = vblankSemaphore;
        if (polled != current) break;
        polled = PollSema(current);
    }
    (void)DIntr();

    int selector = 0;
    int wait_again;
    int pair;
    if (draw[4] != 0u) {
        wait_again = 1;
        pair = 0;
    } else if (draw[7] != 0u) {
        wait_again = 1;
        pair = 0;
    } else {
        const unsigned low = draw[13];
        const unsigned high = draw[9];
        const bool low_neg = (low & 0x80000000u) != 0u;
        const bool high_neg = (high & 0x80000000u) != 0u;
        const bool low_less = low_neg != high_neg ? low_neg : low < high;
        selector = low_less ? 1 : 0;
        const unsigned slot = selector != 0 ? draw[13] : draw[9];
        wait_again = (slot ^ draw[14]) == 0u ? 1 : 0;
        pair = selector << 4;
    }

    const unsigned base = static_cast<unsigned>(pair);
    draw[(24u + base) / 4u] = 1u;
    draw[(28u + base) / 4u] = draw[4];
    draw[(32u + base) / 4u] = draw[5];
    draw[(36u + base) / 4u] = drawFrameNm;
    if (draw[(28u + base) / 4u] != 0u) {
        const unsigned clear_offset = selector != 0 ? 36u : 52u;
        draw[clear_offset / 4u] = 0u;
    }
    draw[3] = 0u;
    const int saved_word = static_cast<int>(draw[4]);
    EIntr();

    if (wait_again != 0) {
        WaitSema(vblankSemaphore);
        switchTo30Counter_798 = 0;
    } else {
        const int old = switchTo30Counter_798;
        switchTo30Counter_798 = old + 1;
        if (old >= 31) {
            engineSetVideoMode__Fb(1);
            switchTo30Counter_798 = 0;
        }
    }

    if (engineDisplayDisable > 0) engineDisplayDisable = engineDisplayDisable - 1;
    const unsigned timer = NORRATH_LOAD32(0x10000000u);
    NORRATH_STORE32(0x10000000u, 0u);
    lastFrameVBlankCount = (timer + 4884u) / 9766u;
    doSuperSample__Fiib(selector, saved_word, argument);

    if (powerOffMode != 0) {
        engineDisplayDisable = 1000;
        for (;;) {
            const int seen = vblankCount;
            if (vblankCount == seen) {
                do WaitSema(vblankSemaphore2);
                while (vblankCount == seen);
            }
            const volatile unsigned char field_now = vblankField;
            (void)field_now;
        }
    }
}
}
