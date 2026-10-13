// Observed InitialiseIopDriver at 0x002e8898, 268 bytes, from the keyboard library the image links in.
// Takes no arguments and returns nothing. It fills a 16-byte request packet inside LocalData (+704):
// halfword at +704 = the low half of GLibData word +64, halfword at +706 = the low half of GLibData word
// +68, halfword at +708 = 0x0106, halfword at +710 = 0, word at +712 = 0. It then calls
// sceSifCallRpc(GLibData, 3, 1, packet, 16, packet, 16, kdm_rpccall_end, packet) and, only when the
// result is not negative, waits on the semaphore id at GLibData word +40.
// Afterwards, a nonzero word at +712 makes the routine spin forever. Otherwise it prints
// "driver version = %d.%02d\n" with the signed byte at +709 and the unsigned byte at +710 and, when the
// halfwords at +708 and +710 differ, spins forever; when they are equal it returns.
// Inferred: the packet carries the two device counts and the version the program expects, and the reply
// fills +710 with the version the IOP module reports; a mismatch is treated as fatal. Unknown: the wire
// meaning of function 3 and of the status word at +712, and the origin of the two counts.
// A host contract replaces the two spin loops by a recorded exit.
extern "C" {
extern unsigned char GLibData[] __attribute__((aligned(8)));   // 0x008ebdd8 is a multiple of 8
extern unsigned char LocalData[] __attribute__((aligned(64)));
int sceSifCallRpc(void* client, unsigned function, unsigned mode, void* send, int send_size, void* receive,
                  int receive_size, void (*end_function)(void*), void* end_argument);
int WaitSema(int semaphore);
int printf(const char* format, ...);
void kdm_rpccall_end(void* argument);
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_spin_forever();
#define NORRATH_SPIN_FOREVER() norrath_host_spin_forever()
#else
#define NORRATH_SPIN_FOREVER() for (;;) {}
#endif

namespace {
struct __attribute__((may_alias)) DriverPacket {
    unsigned short first_count;
    unsigned short second_count;
    short expected_version;
    short reported_version;
    int status;
};
typedef unsigned short __attribute__((may_alias)) AliasedUShort;
typedef int __attribute__((may_alias)) AliasedInt;
}

extern "C" void InitialiseIopDriver() {
    DriverPacket* const packet = reinterpret_cast<DriverPacket*>(LocalData + 704);
    packet->first_count = *reinterpret_cast<AliasedUShort*>(GLibData + 64);
    packet->second_count = *reinterpret_cast<AliasedUShort*>(GLibData + 68);
    packet->expected_version = 0x106;
    packet->reported_version = 0;
    packet->status = 0;
    if (sceSifCallRpc(GLibData, 3, 1, packet, 16, packet, 16, kdm_rpccall_end, packet) >= 0)
        WaitSema(*reinterpret_cast<AliasedInt*>(GLibData + 40));
    if (packet->status != 0) NORRATH_SPIN_FOREVER();
    printf("driver version = %d.%02d\n", static_cast<signed char>(LocalData[709]), LocalData[710]);
    if (packet->expected_version != packet->reported_version) NORRATH_SPIN_FOREVER();
}
