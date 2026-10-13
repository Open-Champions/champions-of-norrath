// Observed: Close__5VIPad at 0x010ff1a8 is 48 bytes and returns nothing. If
// the word at offset 52 is nonzero it clears that word, then calls
// scePadPortClose(word at offset 0, word at offset 4). Nothing happens when
// the word is zero.
// Inferred: offset 0 is the pad port, offset 4 the slot and offset 52 the
// opened flag, from Init__5VIPadii, which stores the port, slot and 1 there.
extern "C" {
int scePadPortClose(int port, int slot);

void Close__5VIPad(void* object) {
    int* words = static_cast<int*>(object);
    if (words[13] != 0) {
        words[13] = 0;
        scePadPortClose(words[0], words[1]);
    }
}
}
