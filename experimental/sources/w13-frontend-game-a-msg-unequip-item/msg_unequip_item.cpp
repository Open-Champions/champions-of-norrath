// Observed msgUnequipItem__FiPQ24item11description at 0x001b1a68, 56 bytes, 400-byte frame buffer.
// It writes an eight-byte message: byte 0 is 2, byte 1 is zero, the halfword at offset 2 is the
// low half of the first argument and the word at offset 4 is the second argument (a pointer to
// an item::description). It sends it with netSendMessage__FiPUci on invMsgHandle
// (0x00486a88), size 8. Same layout as the equip message with kind 2 and no flag byte.
// Unknown: what the receiver does with the fields.
extern "C" int invMsgHandle;
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

namespace {
struct Message {
    unsigned char kind;
    unsigned char flag;
    short first;
    unsigned int item;
    unsigned char rest[392];
};
}

extern "C" void msgUnequipItem__FiPQ24item11description(int first, const void* item) {
    Message message;
    message.kind = 2;
    message.first = static_cast<short>(first);
    message.item = static_cast<unsigned int>(reinterpret_cast<unsigned long>(item));
    message.flag = 0;
    netSendMessage__FiPUci(invMsgHandle, reinterpret_cast<unsigned char*>(&message), 8);
}
