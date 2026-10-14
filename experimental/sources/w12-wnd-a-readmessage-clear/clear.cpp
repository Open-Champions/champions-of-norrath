// Observed: Clear__16VIWndReadMessage at 0x010d1500 is 8 bytes. It stores a zero halfword at
// this+412 and returns nothing.
// Inferred: this+412 is the first code unit of a 16-bit text buffer (the class has AppendMessage
// and Message accessors taking unsigned short text), so this empties the message.
// Unknown: the buffer's capacity and whether it starts at this+412 or only ends there.
extern "C" {
void Clear__16VIWndReadMessage(void* self) {
    *reinterpret_cast<unsigned short*>(static_cast<unsigned char*>(self) + 412) = 0;
}
}
