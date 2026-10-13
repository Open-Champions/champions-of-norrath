// Observed receivePackets__Fv at 0x00133768, 800 bytes in the pinned
// SLUS_205.65 image; no arguments, no result.
// netNetworkMode is not tested before the receive below (the image reads it only
// to guard an unused scan; see Inferred).
// Receive loop: clear broadcastConnectionFlags, then call
//   sceInetRecvFrom(cd, g_net_buf, broadcastConnection, buffer, 1984,
//                   &broadcastConnectionFlags, from, &word, 0)
// with a 16-byte aligned buffer, a 16-byte `from` block and a result word. A
// negative result ends the function. Otherwise the first word h of the buffer
// selects the case, and the loop starts over after each case:
//   h - 0xbd44 in 0..3: when pktHandler[h - 0xbd44] is not null it is called
//       with (buffer + 4, received - 4);
//   0xbd3f: receiveControlPacket(buffer, received, 0);
//   0xbd42: receiveControlPacket(buffer, received, 1);
//   0xbd43: sets netKickedOut to 1 when it is zero;
//   0xbd41: with m = the word at buffer + 4, for each partner index i below
//       nmPartners with bit i of m set, the partner record (17632 bytes each)
//       gets byte 17622 = 1 and word 17624 = -1. Then for each partner whose
//       bytes 17620 and 17621 are both zero and whose byte 17622 is nonzero it
//       zeroes an 80-byte compression object, stores 128.0f in the third word
//       of its first four 16-byte slots, and calls
//       sendControlPacket(0, max(w - 40, 0), w, 0xbd42, i, object, true) where
//       w is the record word at +17596.
//   any other h: nothing.
// Inferred: this is the network receive pump; 0xbd41 is a kick message; the
// object built for sendControlPacket has the same shape as the compression
// objects netInit builds. The scan the image runs first (counting records whose
// byte 17620 is nonzero) has no effect on any later behavior, so it is not
// reproduced. Unknown: what the cases mean beyond the observed stores and what
// the unused bytes of the datagram hold.
extern "C" {
extern signed char netNetworkMode;
extern signed char netKickedOut;
extern int nmPartners;
extern unsigned char partners[];
extern int broadcastConnection;
extern int broadcastConnectionFlags;
extern unsigned char cd[];
extern unsigned char g_net_buf[];
extern void (*pktHandler[])(unsigned char* data, int length);
void* memset(void* destination, int value, unsigned long length);
int sceInetRecvFrom(void* object, void* buffer, int connection, void* data, int length, int* flags,
                    void* address, void* word, int options);
void receiveControlPacket__FPiib(int* buffer, int length, bool flag);
void sendControlPacket__FiiiiiR9CCompressb(int a, int b, int c, int d, int e, void* object, bool flag);

void receivePackets__Fv() {
    alignas(16) unsigned char buffer[1984];
    unsigned char from[16];
    int word[4];
    unsigned char object[80];
    for (;;) {
        broadcastConnectionFlags = 0;
        int received = sceInetRecvFrom(cd, g_net_buf, broadcastConnection, buffer, 1984,
                                       &broadcastConnectionFlags, from, word, 0);
        if (received < 0) return;
        int header = *reinterpret_cast<int*>(buffer);
        if (static_cast<unsigned>(header - 0xbd44) < 4u) {
            void (*handler)(unsigned char*, int) = pktHandler[header - 0xbd44];
            if (handler != 0) handler(buffer + 4, received - 4);
        } else if (header == 0xbd41) {
            int mask = *reinterpret_cast<int*>(buffer + 4);
            int count = nmPartners;
            for (int i = 0; i < count; ++i) {
                if ((mask >> i) & 1) {
                    unsigned char* record = partners + 17632 * i;
                    record[17622] = 1;
                    *reinterpret_cast<int*>(record + 17624) = -1;
                }
            }
            if (count > 0) {
                for (int i = 0; i < count; ++i) {
                    unsigned char* record = partners + 17632 * i;
                    if (record[17620] != 0 || record[17621] != 0) continue;
                    if (record[17622] == 0) continue;
                    memset(object, 0, 16);
                    memset(object + 16, 0, 16);
                    memset(object + 32, 0, 16);
                    memset(object + 48, 0, 16);
                    memset(object + 64, 0, 16);
                    int frame = *reinterpret_cast<int*>(record + 17596);
                    int first = frame - 40;
                    if (first < 0) first = 0;
                    *reinterpret_cast<float*>(object + 8) = 128.0f;
                    *reinterpret_cast<float*>(object + 24) = 128.0f;
                    *reinterpret_cast<float*>(object + 40) = 128.0f;
                    *reinterpret_cast<float*>(object + 56) = 128.0f;
                    *reinterpret_cast<int*>(object + 72) = 0;
                    sendControlPacket__FiiiiiR9CCompressb(0, first, frame, 0xbd42, i, object, true);
                    count = nmPartners;
                }
            }
        } else if (header == 0xbd42) {
            receiveControlPacket__FPiib(reinterpret_cast<int*>(buffer), received, true);
        } else if (header == 0xbd3f) {
            receiveControlPacket__FPiib(reinterpret_cast<int*>(buffer), received, false);
        } else if (header == 0xbd43) {
            if (netKickedOut == 0) netKickedOut = 1;
        }
    }
}
}
