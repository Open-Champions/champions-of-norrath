// Observed msgInvItemSetAttributes__FiiPcPQ24item11description at 0x001b18c0, 64 bytes,
// 400-byte frame buffer. It writes a sixteen-byte message: byte 0 is 8, byte 1 is the low byte
// of the first argument, the halfword at offset 2 is zero, the word at offset 4 is the second
// argument, the word at offset 8 is the third argument (a char pointer) and the word at offset
// 12 is the fourth (an item::description pointer). It sends it with netSendMessage__FiPUci on
// invMsgHandle (0x00486a88), size 16. The third argument is sent as a pointer value, not copied.
// Unknown: what the receiver does with the fields.
extern "C" int invMsgHandle;
extern "C" void netSendMessage__FiPUci(int handle, unsigned char* bytes, int size);

namespace {
struct Message {
    unsigned char kind;
    unsigned char first;
    short zero;
    int second;
    unsigned int text;
    unsigned int item;
    unsigned char rest[384];
};
}

extern "C" void msgInvItemSetAttributes__FiiPcPQ24item11description(int first, int second, char* text, const void* item) {
    Message message;
    message.kind = 8;
    message.first = static_cast<unsigned char>(first);
    message.second = second;
    message.text = static_cast<unsigned int>(reinterpret_cast<unsigned long>(text));
    message.item = static_cast<unsigned int>(reinterpret_cast<unsigned long>(item));
    message.zero = 0;
    netSendMessage__FiPUci(invMsgHandle, reinterpret_cast<unsigned char*>(&message), 16);
}
