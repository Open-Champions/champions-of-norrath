// Observed msg_save__13AutoContainerPc at 0x225030, 24 bytes. Writes the word at
// object+316 to the output pointer and returns the number of bytes written, 4.
// The counterpart is msg_load__13AutoContainerPc. Unknown: the meaning of the word
// beyond the value 2 that the loader tests for.
typedef unsigned Word __attribute__((may_alias));
extern "C" int msg_save__13AutoContainerPc(void* self, char* out) {
    *reinterpret_cast<Word*>(out) = *reinterpret_cast<const Word*>(static_cast<const unsigned char*>(self) + 316);
    return 4;
}
