// Observed: Init__7VILList at 0x010727d0, 20 bytes. With the list as this: stores this+4 at +0, zero at +4 and this itself
// at +8. Nothing is returned that a caller can rely on (the result register still holds this+4).
// Inferred: the three words are laid out so that the pair at +4/+8 reads as a sentinel node (next 0, previous = the tail
// pointer) and the word at +0 as the head link; AddTail reads +8 as the current tail and writes a node's address into the
// first word of that tail, which for an empty list is the head link at this+0. The class name comes only from the symbol.
// Unknown: the size of the list object and any members past +12.
extern "C" {
void Init__7VILList(void* self) {
    unsigned* words = static_cast<unsigned*>(self);
    unsigned address = static_cast<unsigned>(reinterpret_cast<unsigned long>(self));
    words[0] = address + 4;
    words[1] = 0;
    words[2] = address;
}
}
