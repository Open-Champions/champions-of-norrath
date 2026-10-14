/* Observed sceSifResetIop at 0x002dd360, 340 bytes. In order: the notify
 * handlers are run for table 0 and table 1 with a zero argument; the DMA is
 * stopped; sceSifGetReg(0x80000000) is read; a 104-byte packet is built in a
 * local 128-byte buffer (0x008e20c0, named rdata in the symbol table); the
 * packet is written back from the data cache; register 4 is set to 0x40000;
 * and one transfer is queued with sceSifSetDma. A zero result returns 0.
 * Otherwise registers 4 (0x10000), 4 (0x20000), 0x80000002 (0) and
 * 0x80000000 (0) are set in that order and 1 is returned.
 *
 * Packet: byte 0 is 104 and bytes 1..3 are cleared; word 1 is 0; word 2 is
 * 0x80000003; word 3 is not written; word 4 is the string length; word 5 is
 * the mode argument; the string bytes, without a terminator, start at offset
 * 24. The copy has no length bound. The transfer record is the packet, the
 * value read from register 0x80000000, size 104, and attribute 68. What those
 * values mean to the IOP is not established here.
 */
struct SifDmaData {
    void* source;
    unsigned destination;
    unsigned size;
    unsigned attribute;
};

extern "C" {
void sceSifExecNotifyHandler(int use_system_table, unsigned argument);
void sceSifStopDma();
unsigned sceSifGetReg(unsigned reg);
void sceSifSetReg(unsigned reg, unsigned value);
void sceSifWriteBackDCache(void* address, int size);
int sceSifSetDma(SifDmaData* data, int count);
}

static unsigned packet[32];

extern "C" int sceSifResetIop(const char* command, int mode) {
    sceSifExecNotifyHandler(0, 0);
    sceSifExecNotifyHandler(1, 0);
    sceSifStopDma();
    const unsigned remote = sceSifGetReg(0x80000000u);
    unsigned char* bytes = reinterpret_cast<unsigned char*>(packet);
    packet[5] = static_cast<unsigned>(mode);
    int length = 0;
    if (command[0] != 0) {
        do {
            bytes[24 + length] = static_cast<unsigned char>(command[length]);
            ++length;
        } while (command[length] != 0);
    }
    packet[0] = 104;
    packet[1] = 0;
    packet[2] = 0x80000003u;
    packet[4] = static_cast<unsigned>(length);
    SifDmaData transfer = {packet, remote, 104, 68};
    sceSifWriteBackDCache(packet, 104);
    sceSifSetReg(4, 0x40000);
    if (sceSifSetDma(&transfer, 1) == 0) return 0;
    sceSifSetReg(4, 0x10000);
    sceSifSetReg(4, 0x20000);
    sceSifSetReg(0x80000002u, 0);
    sceSifSetReg(0x80000000u, 0);
    return 1;
}
