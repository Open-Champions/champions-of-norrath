// Observed: cdLoadAsyncIOP__FPciT0ii at 0x00166a10 is 220 bytes. The mangled signature is
// (char* name, int destination, char* done, int length, int offset). It does not call cdWait.
// It calls cdDirectoryLookup__FPcPiT1i(name, &sector, &size, 0). A zero result prints the
// format at 0x004633f0 with the name and stores 0 at the address held in the result register,
// which is address 0. A hit uses the file size when length is 0, stores the done pointer in
// cdResult and zero through it, fills the 16-byte command record at 0x006d86b0: word 0 the
// value 2, word 3 the destination, word 1 the lookup sector plus (offset >> 11, arithmetic),
// word 2 (length + 2047) >> 11 (arithmetic). It then calls issueCommand__Fv and stores 1 in the
// byte cdBusy. It does not wait for completion.
// Inferred: command word 0 selects the transfer kind, since cdLoadAsync stores 1 there for an
// EE destination; the destination here being an int fits an IOP address. Not confirmed.
// Unknown: the command word meanings and why the failure store goes to address 0.

extern "C" {
extern signed char cdBusy;
extern unsigned char* cdResult;
extern unsigned command[4];
int cdDirectoryLookup__FPcPiT1i(char* name, int* sector, int* size, int mode);
int scePrintf(const char* format, ...);
void issueCommand__Fv();
#if defined(NORRATH_HOST_CONTRACT)
void norrath_host_store_absolute(unsigned address, unsigned value);
#endif
}

namespace {
void store_absolute(unsigned address, unsigned value) {
#if defined(NORRATH_HOST_CONTRACT)
    norrath_host_store_absolute(address, value);
#else
    // The compiler would otherwise see a constant null address and emit a trap.
    __asm__("" : "+r"(address));
    *reinterpret_cast<volatile unsigned*>(static_cast<__UINTPTR_TYPE__>(address)) = value;
#endif
}
}

extern "C" void cdLoadAsyncIOP__FPciT0ii(char* name, int destination, char* done, int length, int offset) {
    int size;
    int sector;
    const int found = cdDirectoryLookup__FPcPiT1i(name, &sector, &size, 0);
    const int sector_shift = offset >> 11;
    if (found == 0) {
        scePrintf("'%s' not found\n", name);
        store_absolute(static_cast<unsigned>(found), 0u);
    } else {
        if (length == 0) {
            length = size;
        }
        cdResult = reinterpret_cast<unsigned char*>(done);
        *done = 0;
        command[0] = 2;
        command[3] = static_cast<unsigned>(destination);
        command[1] = static_cast<unsigned>(sector + sector_shift);
        command[2] = static_cast<unsigned>((length + 2047) >> 11);
        issueCommand__Fv();
        cdBusy = 1;
    }
}
