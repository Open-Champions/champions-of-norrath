// Observed: padGetUnprocessedData__FiP18padUnprocessedData at 0x00167708 is
// 484 bytes. The port and slot come from index and tapCount (0x00484060,
// signed bytes): below tapCount[0] the port is 0 and the slot is index;
// otherwise the port is 1 and the slot is index minus tapCount[0]. When the
// slot is signed-less than tapCount[port], scePadRead(port, slot, buffer) is
// called with buffer = rdata (0x006e1cc0) plus index times 32, and its result
// is kept; otherwise the result is 0.
// The 8-byte output is cleared by memset, then bytes 2, 3, 4 and 5 are set
// to 0x80. A result of zero or less returns 0 with that output.
// The status byte is buffer[1]. If its high nibble is not 0x7, then
// scePadSetMainMode(port, slot, 1, 3) is called, needActAlign[index]
// (0x00486928) is set to 1, and the call returns 0. Otherwise a nonzero signed
// byte at needActAlign[index] calls scePadSetActAlign(port, slot,
// actAlignData) (0x00484058, six bytes 00 01 ff ff ff ff), clears that byte,
// and returns 0. Otherwise, a status byte other than 0x79 asks
// scePadInfoPressMode(port, slot); a result of 1 calls
// scePadEnterPressMode(port, slot) and returns 0, any other result continues.
// The output is then: half word at +0 = complement of (buffer[2] << 8 |
// buffer[3]); out[4] = buffer[4]; out[5] = buffer[5]; out[2] = buffer[6];
// out[3] = buffer[7]; out[6] = buffer[16]; out[7] = buffer[17]; return 1.
// Inferred: buffer[2..3] is the active-low button word and 4..7 the stick
// bytes of the libpad read packet; the output is an 8-byte record.
// Unknown: the fields of padUnprocessedData beyond these byte positions; the
// unreferenced behaviour of scePadRead, which is not executed here.
extern "C" {
extern signed char tapCount[2];
extern unsigned char rdata[];
extern signed char needActAlign[4];
extern unsigned char actAlignData[6];

int scePadRead(int port, int slot, unsigned char* data);
int scePadSetMainMode(int port, int slot, int offs, int lock);
int scePadSetActAlign(int port, int slot, unsigned char* data);
int scePadInfoPressMode(int port, int slot);
int scePadEnterPressMode(int port, int slot);
void* memset(void* destination, int value, unsigned long size);

int padGetUnprocessedData__FiP18padUnprocessedData(int index, unsigned char* out) {
    int port = 0;
    int slot = index;
    if (!(index < tapCount[0])) {
        slot = index - tapCount[0];
        port = 1;
    }
    unsigned char* const buffer = rdata + index * 32;
    int result = 0;
    if (slot < tapCount[port]) result = scePadRead(port, slot, buffer);
    memset(out, 0, 8);
    out[3] = 0x80;
    out[4] = 0x80;
    out[5] = 0x80;
    out[2] = 0x80;
    if (result <= 0) return 0;
    const unsigned status = buffer[1];
    if ((status & 0xf0u) != 0x70u) {
        scePadSetMainMode(port, slot, 1, 3);
        needActAlign[index] = 1;
        return 0;
    }
    if (needActAlign[index] != 0) {
        scePadSetActAlign(port, slot, actAlignData);
        needActAlign[index] = 0;
        return 0;
    }
    if (status != 0x79u) {
        if (scePadInfoPressMode(port, slot) == 1) {
            scePadEnterPressMode(port, slot);
            return 0;
        }
    }
    const unsigned short buttons = static_cast<unsigned short>(~((buffer[2] << 8) | buffer[3]));
    out[0] = static_cast<unsigned char>(buttons);
    out[1] = static_cast<unsigned char>(buttons >> 8);
    out[4] = buffer[4];
    out[5] = buffer[5];
    out[2] = buffer[6];
    out[3] = buffer[7];
    out[6] = buffer[16];
    out[7] = buffer[17];
    return 1;
}
}
