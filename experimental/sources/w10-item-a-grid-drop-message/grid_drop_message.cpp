// Observed msgInventoryGridDropItem__Fiiciiii at 0x001b1cd8, 68 bytes. It builds
// a 416-byte stack message and sends it with netSendMessage__FiPUci(handle, msg,
// 13), where handle is the word at gp-3944, which is the address of invMsgHandle
// (gp is 0x004879f0 and invMsgHandle 0x00486a88 in the image). Arguments, in order: a, b (int),
// c (char), d, e, f, g (int). Message bytes: [0] = 0, [1] = low byte of a,
// [2..3] = low halfword of d, [4..7] = b, [8..9] = low halfword of e,
// [10] = c, [11] = low byte of f, [12] = low byte of g. Only the first 13 bytes
// are written or sent; the remaining stack bytes are never read.
// Unknown: the meaning of the message fields beyond their position.
extern "C" {
extern int invMsgHandle;
void netSendMessage__FiPUci(int, unsigned char*, int);

void msgInventoryGridDropItem__Fiiciiii(int a, int b, char c, int d, int e, int f, int g) {
    unsigned char msg[416];
    msg[0] = 0;
    msg[1] = static_cast<unsigned char>(a);
    *reinterpret_cast<short*>(msg + 2) = static_cast<short>(d);
    *reinterpret_cast<int*>(msg + 4) = b;
    *reinterpret_cast<short*>(msg + 8) = static_cast<short>(e);
    msg[10] = static_cast<unsigned char>(c);
    msg[11] = static_cast<unsigned char>(f);
    msg[12] = static_cast<unsigned char>(g);
    netSendMessage__FiPUci(invMsgHandle, msg, 13);
}
}
