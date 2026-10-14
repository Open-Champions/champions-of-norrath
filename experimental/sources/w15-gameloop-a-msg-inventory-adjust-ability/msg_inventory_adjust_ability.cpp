// Observed msgInventoryAdjustAbility__Fii at 0x001b1900, 52 bytes, 416-byte frame. It builds a four-byte message in
// a local buffer: byte 0 is 6, byte 1 is the low byte of the first argument and the halfword at offset 2 is the low
// 16 bits of the second argument. It then calls netSendMessage__FiPUci(invMsgHandle, buffer, 4) and returns nothing.
// invMsgHandle is a gp-relative word read at call time.
// Inferred: type 6 selects an "adjust ability" message and the arguments are an index and a signed amount (the
// names come from the symbol table). Unknown: the receiving handler and the exact meaning of each field.
extern "C" {
extern int invMsgHandle;
void netSendMessage__FiPUci(int handle, unsigned char* data, int length);

void msgInventoryAdjustAbility__Fii(int index, int amount) {
    struct {
        unsigned char type;
        unsigned char index;
        short amount;
    } message;
    message.type = 6;
    message.index = static_cast<unsigned char>(index);
    message.amount = static_cast<short>(amount);
    netSendMessage__FiPUci(invMsgHandle, reinterpret_cast<unsigned char*>(&message), 4);
}
}
