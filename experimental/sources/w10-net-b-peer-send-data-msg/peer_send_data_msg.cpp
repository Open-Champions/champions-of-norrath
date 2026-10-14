// Observed peerSendDataMsg__FPciiP14sceInetAddressi at 0x0012d660, 304 bytes in
// the pinned SLUS_205.65 image; arguments (data, size, sequence, address,
// limit), returning an int.
//  1. Checksum: starts at 535. For index i below `size`, with c = the signed
//     byte data[i]: odd i adds (3 * c) ^ 0x30e15f; even i subtracts
//     (c & 0x148f37) * (i & 7). After each step the signed first byte data[0]
//     is added (not data[i]). The checksum is then shifted left 16 and ORed
//     with `sequence`.
//  2. Packet on the stack, words: the word at launchPeers + 252 * s + 64 where
//     s is the signed byte netGameLaunchInfo[6]; the constant 4; the checksum
//     word; then `size` bytes copied from `data` (memcpy).
//  3. sceInetSendTo(cd, lnet_buf, udpLaunchConnection, packet, size + 12,
//     &zero, address, limit, 100) with a local word set to zero, and the
//     checksum word read back from the packet after the call is returned.
// Inferred: this is one data chunk of the launch lobby (the authored
// netDoNewLaunch passes chunks through it); the first word identifies the
// local launch entry. Unknown: why the first byte is added on every step
// (observed behavior, kept as is). The image's stack buffer takes at most 1300
// payload bytes; larger sizes are not handled by either version.
extern "C" {
extern unsigned char cd[];
extern void* lnet_buf;
extern int udpLaunchConnection;
extern unsigned char netGameLaunchInfo[];
extern unsigned char launchPeers[];
void* memcpy(void* destination, const void* source, unsigned long length);
int sceInetSendTo(void* object, void* buffer, int connection, void* data, int length, int* flags,
                  void* address, int first_limit, int second_limit);

int peerSendDataMsg__FPciiP14sceInetAddressi(char* data, int size, int sequence, void* address, int limit) {
    int sum = 535;
    for (int i = 0; i < size; ++i) {
        int c = data[i];
        if (i & 1) sum += (c * 3) ^ 0x30e15f;
        else sum -= (c & 0x148f37) * (i & 7);
        sum += data[0];
    }
    int word[3 + 325];
    int zero = 0;
    int checksum = (sum << 16) | sequence;
    int index = static_cast<signed char>(netGameLaunchInfo[6]);
    word[0] = *reinterpret_cast<int*>(launchPeers + 252 * index + 64);
    word[1] = 4;
    word[2] = checksum;
    memcpy(word + 3, data, size);
    sceInetSendTo(cd, lnet_buf, udpLaunchConnection, word, size + 12, &zero, address, limit, 100);
    return word[2];
}
}
