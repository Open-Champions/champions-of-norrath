// Observed msgEquipItem__FiPQ24item11descriptioni at 0x001b1a30, 56 bytes, 400-byte frame buffer.
// It writes an eight-byte message: byte 0 is 1, byte 1 is the low byte of the third argument,
// the halfword at offset 2 is the low half of the first argument, and the word at offset 4 is
// the second argument (a pointer to an item::description). It sends it with
// netSendMessage__FiPUci on the handle word invMsgHandle (0x00486a88), size 8.
// Unknown: what the receiver does with the three fields; the meaning of the third argument.
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

extern "C" void msgEquipItem__FiPQ24item11descriptioni(int first, const void* item, int flag) {
    Message message;
    message.kind = 1;
    message.first = static_cast<short>(first);
    message.flag = static_cast<unsigned char>(flag);
    message.item = static_cast<unsigned int>(reinterpret_cast<unsigned long>(item));
    netSendMessage__FiPUci(invMsgHandle, reinterpret_cast<unsigned char*>(&message), 8);
}
