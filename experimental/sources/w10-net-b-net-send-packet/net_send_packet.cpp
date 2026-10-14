// Observed netSendPacket__FiiPUci at 0x00129fe8, 424 bytes in the pinned
// SLUS_205.65 image; arguments (kind, mask, data, length), no result.
//  1. When netNetworkMode is zero it returns at once.
//  2. It builds a packet on the stack: a 32-bit word 0xbd44 + kind, then
//     `length` bytes copied from `data` (memcpy; the stack buffer holds 512).
//  3. For each partner index i below nmPartners (reloaded every iteration)
//     whose bit i of `mask` is set, with the partner record at
//     partners + 17632 * i:
//       - if the record byte at +17620 is nonzero and pktHandler[kind] is not
//         null, it calls that handler with (packet + 4, length), the copy without the header word;
//       - else if the record byte at +17620 is zero and the byte at +17621 is
//         zero, it clears broadcastConnectionFlags, builds a 16-byte block of
//         zeroes with bytes 4..7 taken from record bytes 0..3 and calls
//         sceInetSendTo(cd, g_net_buf, broadcastConnection, packet, length + 4,
//         0, block, <word at record +4>, 1000);
//       - otherwise nothing is done for that partner.
// Inferred: kind selects a message class; the record word at +4 is the peer's
// port value stored by the lobby code (the lobby copies the received "word"
// there). Unknown: what the handler table holds and which player it serves.
extern "C" {
extern signed char netNetworkMode;
extern int nmPartners;
extern unsigned char partners[];
extern int broadcastConnection;
extern int broadcastConnectionFlags;
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern void (*pktHandler[])(unsigned char* packet, int length);
void* memcpy(void* destination, const void* source, unsigned long length);
void* memset(void* destination, int value, unsigned long length);
int sceInetSendTo(void* object, void* buffer, int connection, void* data, int length, int flags,
                  void* address, int first_limit, int second_limit);

void netSendPacket__FiiPUci(int kind, int mask, unsigned char* data, int length) {
    if (netNetworkMode == 0) return;
    unsigned char packet[512];
    unsigned char block[16];
    *reinterpret_cast<int*>(packet) = 0xbd44 + kind;
    memcpy(packet + 4, data, length);
    for (int index = 0; index < nmPartners; ++index) {
        if (((mask >> index) & 1) == 0) continue;
        unsigned char* record = partners + 17632 * index;
        if (record[17620] != 0) {
            void (*handler)(unsigned char*, int) = pktHandler[kind];
            if (handler != 0) handler(packet + 4, length);
        } else if (record[17621] == 0) {
            int port = *reinterpret_cast<int*>(record + 4);
            broadcastConnectionFlags = 0;
            memset(block, 0, 16);
            for (int b = 0; b < 4; ++b) block[4 + b] = record[b];
            sceInetSendTo(cd, g_net_buf, broadcastConnection, packet, length + 4, 0, block, port, 1000);
        }
    }
}
}
