// Observed: padClear__Fv at 0x00167660 is 168 bytes. For each index 0..3 it
// stores zero words at +0, +4, +8, +16 and +20 of a 24-byte record, advancing
// from rumphole (0x006e1c60, 96 bytes to the next symbol rdata). The word at
// +12 is not written. It then stores zero bytes at actSendData+0 and +1
// (0x00486930, 8 bytes to the next symbol). The port and slot come from
// tapCount (0x00484060), read as signed bytes: while index is below
// tapCount[0] the port is 0 and the slot is index; otherwise the port is 1 and
// the slot is index minus tapCount[0]. When the slot is signed-less than
// tapCount[port], scePadSetActDirect(port, slot, actSendData) is called.
// Inferred: the 24-byte records are four per-pad vibration states.
// Unknown: the meaning of the record fields; the return value of
// scePadSetActDirect is ignored.
extern "C" {
extern unsigned rumphole[24];
extern unsigned char actSendData[8];
extern signed char tapCount[2];

int scePadSetActDirect(int port, int slot, unsigned char* data);

void padClear__Fv() {
    for (int index = 0; index < 4; ++index) {
        unsigned* const record = rumphole + index * 6;
        record[2] = 0;
        record[0] = 0;
        record[1] = 0;
        record[4] = 0;
        record[5] = 0;
        actSendData[0] = 0;
        actSendData[1] = 0;
        int port = 1;
        int slot = index - tapCount[0];
        if (index < tapCount[0]) {
            port = 0;
            slot = index;
        }
        if (slot < tapCount[port]) scePadSetActDirect(port, slot, actSendData);
    }
}
}
