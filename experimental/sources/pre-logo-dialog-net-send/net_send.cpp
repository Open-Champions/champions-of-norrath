// Observed netSendMessage__FiPUci at 0x00134f68, 256 bytes.
// A zero netNetworkMode byte calls msgHandler[handle] with the byte pointer
// and the length, then returns. Any other mode, including 0x80, asks
// netGetLocalPlayerNm for index 0 and writes a ring in that partner record.
// The record stride is 17632. The counter word is at byte 17580. The first
// two stored bytes use signed division by 8191. Payload bytes and the stored
// counter use a bitwise and with 0x1fff. partners, msgHandler, and
// netGetLocalPlayerNm are not defined here.
extern "C" unsigned char netNetworkMode;
extern "C" int netGetLocalPlayerNm__Fi(int index);
extern "C" unsigned char partners[];
extern "C" void (*msgHandler[])(unsigned char* bytes, int length);

extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int length) {
    if (netNetworkMode == 0) {
        msgHandler[handle](bytes, length);
        return;
    }
    const int player = netGetLocalPlayerNm__Fi(0);
    unsigned char* record = partners + player * 17632;
    unsigned char* ring = record + 12;
    const unsigned counter = static_cast<unsigned>(record[17580]) |
              (static_cast<unsigned>(record[17581]) << 8) |
              (static_cast<unsigned>(record[17582]) << 16) |
              (static_cast<unsigned>(record[17583]) << 24);
    auto store_mod = [&](unsigned bits, unsigned char value) {
        const int signed_bits = static_cast<int>(bits);
        const int remainder = signed_bits % 8191;
        ring[remainder + 9376] = value;
    };
    store_mod(counter, static_cast<unsigned char>(handle));
    store_mod(counter + 1u, static_cast<unsigned char>(length));
    if (length > 0) {
        for (int index = 0; index < length; ++index) {
            const unsigned slot = (counter + 2u + static_cast<unsigned>(index)) & 0x1fffu;
            ring[static_cast<int>(slot) + 9376] = bytes[index];
        }
    }
    const unsigned updated = (counter + static_cast<unsigned>(length) + 2u) & 0x1fffu;
    record[17580] = static_cast<unsigned char>(updated);
    record[17581] = static_cast<unsigned char>(updated >> 8);
    record[17582] = static_cast<unsigned char>(updated >> 16);
    record[17583] = static_cast<unsigned char>(updated >> 24);
}
